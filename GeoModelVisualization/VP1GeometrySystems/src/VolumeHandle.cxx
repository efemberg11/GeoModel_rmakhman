/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

#include "VP1GeometrySystems/VolumeHandle.h"
#include "VP1GeometrySystems/VolumeHandleSharedData.h"
#include "VP1GeometrySystems/PhiSectorManager.h"
#include "VP1GeometrySystems/VisAttributes.h"
#include "VP1GeometrySystems/VP1GeoTreeView.h"
#include "VP1GeometrySystems/GeoSysController.h"

#include "VP1Base/VP1ExtraSepLayerHelper.h"
#include "VP1Base/VP1Msg.h"
// #include "VP1Base/VP1QtInventorUtils.h"
//#include "VP1Utils/VP1LinAlgUtils.h"
#include "VP1HEPVis/nodes/SoTransparency.h"
#include "VP1HEPVis/nodes/SoPolyhedron.h"
#include "VP1HEPVis/nodes/SoPcons.h"
#include "VP1HEPVis/VP1HEPVisUtils.h"

#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTexture2.h>

#include <QHash>
#include <QBuffer>
#include <QByteArray>
#include <QPair>
#include <QSet>
#include <QMap>
#include <QDataStream>
#include <unistd.h>
//____________________________________________________________________
class VolumeHandle::Imp {
public:
  Imp(VolumeHandleSharedData * the_cd, const GeoPVConstLink& the_pV, const SbMatrix& the_ac)
    : commondata(the_cd), pV(the_pV),accumTrans(the_ac),attachsepHelper(0),attachlabelSepHelper(0),nodesep(0), material(0), label_sep(0), labels(0), isattached(false) {}

  VolumeHandleSharedData * commondata;
  GeoPVConstLink pV;
  const SbMatrix accumTrans;//FIXME: Use pointer - and free once children are created AND nodesep has been build. Or just construct on the fly!

  VP1ExtraSepLayerHelper * attachsepHelper;
  VP1ExtraSepLayerHelper * attachlabelSepHelper;

  SoSeparator * nodesep;
  SoMaterial * material;
  SoSeparator* label_sep;
  int labels;
  QList<int> labelsPosOffsets;

  bool isattached;
  void attach(VolumeHandle*vh);
  void detach();

  class VolState {
  public:
    int nZappedChildren() const { return m_zappedChildren.count(); }
    int nExpandedChildren() const { return m_expandedChildren.count(); }
    QMap<quint32,VolState> m_expandedChildren;
    QSet<quint32> m_zappedChildren;
  };

  static VolState getChildStates(const VolumeHandle*theclass );
  static void applyChildStates(const VolState&,VolumeHandle*theclass);
  static bool hasNonStandardShapeChildren(const SoGroup*g);

};


QDataStream & operator<< ( QDataStream & out, const VolumeHandle::Imp::VolState & vs ) {
  out << vs.m_expandedChildren;
  out << vs.m_zappedChildren;
  return out;
}
QDataStream & operator>> ( QDataStream & in, VolumeHandle::Imp::VolState & vs ) {
  in >> vs.m_expandedChildren;
  in >> vs.m_zappedChildren;
  return in;
}

//____________________________________________________________________
VolumeHandle::VolumeHandle(VolumeHandleSharedData * cd,VolumeHandle * parent, const GeoPVConstLink& pV, int childNumber, const SbMatrix& accumTrans)
  : m_d(new Imp(cd,pV,accumTrans)), m_childNumber(childNumber), m_nchildren(childNumber>=0?pV->getNChildVols():0), m_parent(parent),
    m_state(VP1GeoFlags::CONTRACTED)
{
  if (cd) {
    cd->ref();
    if (!haveParentsNotExpanded())
      m_d->attach(this);
  }
}

//____________________________________________________________________
VolumeHandle::~VolumeHandle()
{
  if (m_d->commondata) {
    setState(VP1GeoFlags::ZAPPED);
    m_d->commondata->removeZappedVolumesFromGui(this);
    if (childrenAreInitialised()) {
      VolumeHandleListItr it, itE = m_children.end();
      for (it = m_children.begin(); it!=itE; ++it)
        delete *it;
      m_children.clear();
    }
    if (m_d->material)
      m_d->material->unref();
    if (m_d->nodesep)
      m_d->nodesep->unref();
    if (m_d->label_sep)
      m_d->label_sep->unref();
    m_d->commondata->unref();
  }
  delete m_d;
}

//____________________________________________________________________
void VolumeHandle::initialiseChildren()
{
  if (childrenAreInitialised())
    return;

  assert(m_nchildren);

  //Loop over children:
  m_children.reserve(m_nchildren);
  unsigned ichild(0);
  GeoVolumeCursor av(m_d->pV);
  while (!av.atEnd()) {

    //Add transformation between parent and child to find the complete transformation of the child:
    const GeoTrf::Transform3D::MatrixType  mtx=av.getTransform().matrix();
    SbMatrix matr(mtx(0,0),mtx(1,0),mtx(2,0),mtx(3,0),  // Beware, conventions
		  mtx(0,1),mtx(1,1),mtx(2,1),mtx(3,1),  // differ!
		  mtx(0,2),mtx(1,2),mtx(2,2),mtx(3,2),
		  mtx(0,3),mtx(1,3),mtx(2,3),mtx(3,3));

    matr.multRight(m_d->accumTrans);
    m_children.push_back(new VolumeHandle(m_d->commondata,this,av.getVolume(),ichild++,matr));
    m_children.back()->expandMothersRecursivelyToNonEther();
    av.next();
  }

  assert(ichild==m_nchildren&&m_children.size()==m_nchildren);

}

//____________________________________________________________________
GeoPVConstLink VolumeHandle::geoPVConstLink() const
{
  return m_d->pV;
}

//____________________________________________________________________
QString VolumeHandle::getName() const
{
  return m_d->pV->getLogVol()->getName().c_str();
}

//____________________________________________________________________
std::string VolumeHandle::getNameStdString() const
{
  return m_d->pV->getLogVol()->getName();
}

//____________________________________________________________________
bool VolumeHandle::hasName(const std::string& n) const
{
  return m_d->pV->getLogVol()->getName() == n;
}

//____________________________________________________________________
SoMaterial * VolumeHandle::material()
{
  VP1Msg::messageDebug2("VolumeHandle::material() - LogVol name: " + QString::fromStdString(m_d->pV->getLogVol()->getName()));
  // if it's not the first time here and
  // the material has been assigned already, then return that
  if (m_d->material)
    return m_d->material;

  // if it's the first timne here, the material has not been assigned yet, then...

  //Then, see if the "databases" of defined volumes/material know about this volume:
  SoMaterial * mat = m_d->commondata->volVisAttributes()->get(m_d->pV->getLogVol()->getName());
  if (mat) {
    m_d->material = mat;
    m_d->material->ref();
    return m_d->material;
  }
  //Then, see if the "databases" of defined material know about this volume:
  mat = m_d->commondata->matVisAttributes()->get(m_d->pV->getLogVol()->getMaterial()->getName());
  if (mat) {
    m_d->material = mat;
    m_d->material->ref();
    return m_d->material;
  }

  //Apparently not. Thus, we have to define it.
  // We now have two ways of finding a material: We can
  //take a system dependent fallback material, or we can take the
  //material of the parent.

  mat = m_d->commondata->fallBackTopLevelMaterial();
  if (mat) {
    m_d->material = mat;
    m_d->material->ref();
    return m_d->material;
  }

  if (m_parent) {
    m_d->material = m_parent->material();
    assert(m_d->material);
    m_d->material->ref();
    return m_d->material;
  }

  m_d->material = m_d->commondata->matVisAttributes()->get("DEFAULT");
  assert(m_d->material);
  m_d->material->ref();
  return m_d->material;
}

//____________________________________________________________________
SoSeparator * VolumeHandle::nodeSoSeparator() const
{
  return m_d->nodesep;
}

//____________________________________________________________________
void VolumeHandle::ensureBuildNodeSep()
{
  VP1Msg::messageDebug3("VolumeHandle::ensureBuildNodeSep()");
  if (m_d->nodesep && m_d->label_sep)
    return;

  m_d->label_sep = new SoSeparator;
  m_d->label_sep->ref();

  m_d->nodesep = new SoSeparator;//FIXME: rendercaching??
  //   m_d->nodesep->renderCaching.setValue(SoSeparator::ON);
  //   m_d->nodesep->boundingBoxCaching.setValue(SoSeparator::ON);
  m_d->nodesep->ref();//Since we are keeping it around irrespective of whether it is attached or not.

  //Transform:
  {
    SoTransform *xf=new SoTransform();
    xf->setMatrix(m_d->accumTrans);
    m_d->nodesep->addChild(xf);
  }


  //VP1Msg::messageDebug("calling toShapeNode()...");
  bool shapeIsKnown;
  SoNode * shape = m_d->commondata->toShapeNode(m_d->pV, &shapeIsKnown);//NB: Ignore contained transformation of GeoShapeShifts.
  static const char *unknownShapeTextureFile[]={"/usr/share/gmex/unknownShape.jpg","/usr/local/share/gmex/unknownShape.jpg"};
  SoTexture2 *skin=nullptr;
  if (!shapeIsKnown) {
    std::cout << "Unknown shape:" << m_d->pV->getLogVol()->getShape()->type() << std::endl;
    for (int trial=0;trial<2;trial++) {
      if (!access(unknownShapeTextureFile[trial],R_OK)) {
	skin = new SoTexture2;
	skin->filename.setValue(unknownShapeTextureFile[trial]);
	skin->model=SoTexture2::REPLACE;
	break;
      }
    }
  }
  if (!shape) {
    m_d->nodesep->removeAllChildren();
    return;
  }

  //What phi sector do we belong in?
  int iphi = m_d->commondata->phiSectorManager()->getVolumeType(m_d->accumTrans, shape);

  if (iphi >= -1 ) {
    //VP1Msg::messageDebug("Cylinders [iphi >= -1]...");
    //Substitute shapes that are essentially cylinders with such. This
    //can be done safely since this tube won't need
    //phi-slicing and is done to gain render performance.
    if ( m_d->pV->getLogVol()->getShape()->typeID()==GeoTube::getClassTypeID() )
    {
      //VP1Msg::messageDebug("GeoTube...");
      const GeoTube * geotube = static_cast<const GeoTube*>(m_d->pV->getLogVol()->getShape());
      if (geotube->getRMin()==0.0)
        shape = m_d->commondata->getSoCylinderOrientedLikeGeoTube(geotube->getRMax(),geotube->getZHalfLength());
    }
    else if ( m_d->pV->getLogVol()->getShape()->typeID()==GeoTubs::getClassTypeID() )
    {
      //VP1Msg::messageDebug("GeoTubs...");
      const GeoTubs * geotubs = static_cast<const GeoTubs*>(m_d->pV->getLogVol()->getShape());
      if (geotubs->getRMin()==0.0 && geotubs->getDPhi() >= 2*M_PI-1.0e-6)
        shape = m_d->commondata->getSoCylinderOrientedLikeGeoTube(geotubs->getRMax(),geotubs->getZHalfLength());
    }
  }

  //In the case of a GeoShapeShift we add its contained transformation here:
  //Fixme: Remember to use this extra transformation for phisector cuts also!
  if (m_d->pV->getLogVol()->getShape()->typeID()==GeoShapeShift::getClassTypeID()) {
    const GeoTrf::Transform3D::MatrixType &mtx=dynamic_cast<const GeoShapeShift*>(m_d->pV->getLogVol()->getShape())->getX().matrix();
    SbMatrix matr(mtx(0,0),mtx(1,0),mtx(2,0),mtx(3,0),  // Beware, conventions
		  mtx(0,1),mtx(1,1),mtx(2,1),mtx(3,1),  // differ!
		  mtx(0,2),mtx(1,2),mtx(2,2),mtx(3,2),
		  mtx(0,3),mtx(1,3),mtx(2,3),mtx(3,3));
    SoTransform *xf=new SoTransform();
    m_d->nodesep->addChild(xf);
    xf->setMatrix(matr);
  }
  //Add shape child(ren) and get the separator (helper) where we attach the nodesep when volume is visible:
  if (iphi >= -1) {
    if (!shapeIsKnown && skin) m_d->nodesep->addChild(skin);
    m_d->nodesep->addChild(shape);
    m_d->attachsepHelper = m_d->commondata->phiSectorManager()->getSepHelperForNode(m_d->commondata->subSystemFlag(), iphi);
    m_d->attachlabelSepHelper = m_d->commondata->phiSectorManager()->getLabelSepHelperForNode(m_d->commondata->subSystemFlag(), iphi);
  } else {
    SoSwitch * sw = new SoSwitch;
    if (!shapeIsKnown && skin) m_d->nodesep->addChild(skin);
    sw->addChild(shape);
    SoSeparator * sep_slicedvols = new SoSeparator;
    sw->addChild(sep_slicedvols);
    sw->whichChild = 0;
    m_d->nodesep->addChild(sw);
    m_d->attachsepHelper = m_d->commondata->phiSectorManager()->registerVolumeAroundZAxis( m_d->commondata->subSystemFlag(), sw, m_d->accumTrans );
    //FIXME - what about labels?
  }

  //Register shape to volume handle connection (for user interactions):
  //NB: "shape" might be shared between several volumes, so we use the separator above for the actual link!
  //(this must be done last as it also sets outline defaults)
  m_d->commondata->registerNodeSepForVolumeHandle(m_d->nodesep,this);

  //VP1Msg::messageDebug("VolumeHandle::ensureBuildNodeSep() - DONE.");
}

//____________________________________________________________________
void VolumeHandle::Imp::attach(VolumeHandle*vh)
{
  VP1Msg::messageDebug3("VolumeHandle::Imp::attach() - name: " + vh->getName());
  if (!isattached) {
    vh->ensureBuildNodeSep();
    if (attachsepHelper) {
      VP1Msg::messageDebug3("adding node...");
      attachsepHelper->addNodeUnderMaterial(nodesep,vh->material());
    }
    if (attachlabelSepHelper) {
      VP1Msg::messageDebug3("adding label...");
      attachlabelSepHelper->addNode(label_sep);
    }
    isattached=true;
    commondata->volumeBrowser()->scheduleUpdateOfAllNonHiddenIndices();//Browser need to change e.g. colour for this volume
  }
  //VP1Msg::messageDebug("attach: DONE.");
}

//____________________________________________________________________
void VolumeHandle::Imp::detach()
{
  if (isattached) {
    if (attachsepHelper)
      attachsepHelper->removeNodeUnderMaterial(nodesep,material);
    if (attachlabelSepHelper)
      attachlabelSepHelper->removeNode(label_sep);
    isattached=false;
    commondata->volumeBrowser()->scheduleUpdateOfAllNonHiddenIndices();//Browser need to change e.g. colour for this volume
  }
}

//____________________________________________________________________
void VolumeHandle::setState( const VP1GeoFlags::VOLSTATE& state )
{
  if (m_state==state) {
    return;
  }

  //Update state flag and presence in GUI lists:
  VP1GeoFlags::VOLSTATE oldstate = m_state;
  m_state = state;
  if (oldstate==VP1GeoFlags::ZAPPED)
    m_d->commondata->removeZappedVolumesFromGui(this);
  else if (state==VP1GeoFlags::ZAPPED)
    m_d->commondata->addZappedVolumeToGui(this);

  //Only thing left is visibility updates (i.e. attachment to 3D scenegraph).

  // if (haveParentsNotExpanded()) {
  //   //No visibility updates necessary
  //   assert(!m_d->isattached);
  //   return;
  // }

  //We might need visibility updates. Which ones depend on the
  //particular change of state:

  if (state==VP1GeoFlags::CONTRACTED) {
    {
      for (auto d=childrenBegin();d!=childrenEnd();d++) {
	if ((*d)->isEther()) (*d)->setState(VP1GeoFlags::CONTRACTED);
      }
      if (!isEther()) {
	m_d->attach(this);
	detachAllContractedChildren();
      }
      else {
	bool otherThanEther=false;
	VolumeHandle *p=parent();
	while (p) {
	  if (!p->isEther()) {
	    otherThanEther=true;
	    break;
	  }
	  p=p->parent();
	}
	if (!otherThanEther) {
	  return; // Nothing but ether, up there. Return;
	}
	m_d->attach(this);
	detachAllContractedChildren();
	{
	  VolumeHandle *p=parent();
	  if (p) p->setState(VP1GeoFlags::CONTRACTED);
	}
      }
    }

  }
  else if (state==VP1GeoFlags::EXPANDED) {
    if (oldstate==VP1GeoFlags::CONTRACTED)
      m_d->detach();
    attachAllContractedChildren();
    for (auto d=childrenBegin();d!=childrenEnd();d++) {
      if ((*d)->isEther()) (*d)->setState(VP1GeoFlags::EXPANDED);
    }
  }
  else {
    assert(state==VP1GeoFlags::ZAPPED);
    // VP1GeoFlags::CONTRACTED -> VP1GeoFlags::ZAPPED: Hide this.
    // VP1GeoFlags::EXPANDED -> VP1GeoFlags::ZAPPED: Hide all children.
    if (oldstate==VP1GeoFlags::CONTRACTED)
      m_d->detach();
    else
      detachAllContractedChildren();
  }
}

//____________________________________________________________________
void VolumeHandle::contractDaughtersRecursively()
{
  if (!childrenAreInitialised())
    return;//Fine to abort since children starts in contracted state
	   //if initialised at a later time.
  VolumeHandleListItr childItrE = m_children.end();
  for (VolumeHandleListItr childItr = m_children.begin(); childItr!=childItrE; ++childItr) {
    (*childItr)->setState(VP1GeoFlags::CONTRACTED);
    (*childItr)->contractDaughtersRecursively();
  }
}

//____________________________________________________________________
bool VolumeHandle::haveParentsNotExpanded() const
{
  return m_parent ? ( m_parent->state()==VP1GeoFlags::EXPANDED ? m_parent->haveParentsNotExpanded() : true )  : false;
}

//____________________________________________________________________
void VolumeHandle::attachAllContractedChildren() {

  if (!m_nchildren)
    return;
  if (!childrenAreInitialised())
    initialiseChildren();

  VolumeHandleListItr childItrE = m_children.end();
  for (VolumeHandleListItr childItr = m_children.begin(); childItr!=childItrE; ++childItr) {
    if ((*childItr)->state()==VP1GeoFlags::CONTRACTED)
      (*childItr)->m_d->attach(*childItr);
    else if ((*childItr)->state()==VP1GeoFlags::EXPANDED)
      (*childItr)->attachAllContractedChildren();
  }
}

//____________________________________________________________________
void VolumeHandle::detachAllContractedChildren() {
  if (!m_nchildren)
    return;
  if (!childrenAreInitialised())
    return;//Since children not initialised won't have been attached!

  VolumeHandleListItr childItrE = m_children.end();
  for (VolumeHandleListItr childItr = m_children.begin(); childItr!=childItrE; ++childItr) {
    if ((*childItr)->state()==VP1GeoFlags::CONTRACTED)
      (*childItr)->m_d->detach();
    else if ((*childItr)->state()==VP1GeoFlags::EXPANDED)
      (*childItr)->detachAllContractedChildren();
  }
}

//____________________________________________________________________
int VolumeHandle::copyNumber() const
{
  GeoPVConstLink parent_pV;
  if (m_parent) {
    parent_pV = m_parent->geoPVConstLink();
  } else {
    parent_pV = m_d->commondata->geoPVConstLinkOfTreeTopsMother();
    if (parent_pV==m_d->pV) {
      //To follow historical behaviour, we always assume that
      //tree-tops (in the GeoModel sense) have a copy number of 0.
      return 0;
    }
  }
  GeoVolumeCursor av(parent_pV);
  int i(0);//We need to check the childNumber since volumes in a replica have same volume link
  while (!av.atEnd()) {
    if (m_childNumber==i&&m_d->pV==av.getVolume()) {
       Query<int> Qint = av.getId();
       return Qint.isValid() ? int(Qint) : -1;//-1 for "Invalid"
    }
    av.next();
    ++i;
  }
  return -2;//error
}

//____________________________________________________________________
quint32 VolumeHandle::hashID() const
{
  //  return qHash( QPair<QString,quint32>(getName(),copyNumber()));//NOT UNIQUE!!
  return qHash( QPair<QString,quint32>(getName(),childNumber()));
}

//____________________________________________________________________
const SbMatrix& VolumeHandle::getGlobalTransformToVolume() const {
  return m_d->accumTrans;
}

//____________________________________________________________________
SbMatrix VolumeHandle::getLocalTransformToVolume() const {
  if (!m_parent)
    return m_d->accumTrans;
  return m_parent->getGlobalTransformToVolume().inverse().multRight(m_d->accumTrans);
}

//____________________________________________________________________
bool VolumeHandle::isAttached() const
{
  return m_d->isattached;
}

//____________________________________________________________________
const GeoMaterial * VolumeHandle::geoMaterial() const {
  return geoPVConstLink()->getLogVol()->getMaterial();
}

//____________________________________________________________________
bool VolumeHandle::isEther() const
{
  return QString(geoMaterial()->getName().c_str()).endsWith("Ether") ||
    QString(geoMaterial()->getName().c_str()).endsWith("HyperUranium");
}

//____________________________________________________________________
void VolumeHandle::expandMothersRecursivelyToNonEther() {

  if (!nChildren()||!isEther()) {
    return;
  }

  setState(VP1GeoFlags::ZAPPED);
  initialiseChildren();
  VolumeHandleListItr childItrE = m_children.end();
  for (VolumeHandleListItr childItr = m_children.begin(); childItr!=childItrE; ++childItr) {
    (*childItr)->expandMothersRecursivelyToNonEther();
  }
  setState(VP1GeoFlags::EXPANDED);
}

//____________________________________________________________________
QByteArray VolumeHandle::getPersistifiableState() const
{
  QByteArray ba;
  QBuffer buffer(&ba);
  buffer.open(QIODevice::WriteOnly);

  QDataStream out(&buffer);
  out << (qint32)0;//version
  out << qint32(m_state==VP1GeoFlags::CONTRACTED? 0 : (m_state==VP1GeoFlags::EXPANDED?1:2));
  out << Imp::getChildStates(this);

  buffer.close();
  return ba;

}

//____________________________________________________________________
void VolumeHandle::applyPersistifiableState( QByteArray ba )
{
  QBuffer buffer(&ba);
  buffer.open(QIODevice::ReadOnly);
  QDataStream state(&buffer);
  qint32 version;
  state >> version;
  if (version!=0)
    return;//We ignore wrong versions silently here.
  qint32 st;
  state >> st;
  VP1GeoFlags::VOLSTATE newstate(VP1GeoFlags::CONTRACTED);
  if (st == 2)
    newstate = VP1GeoFlags::ZAPPED;
  else if (st == 1)
    newstate = VP1GeoFlags::EXPANDED;

  Imp::VolState vs;
  state >> vs;
  buffer.close();

  if (newstate==VP1GeoFlags::EXPANDED) {
    setState(VP1GeoFlags::ZAPPED);
    m_d->applyChildStates(vs,this);
  }
  setState(newstate);
}

//____________________________________________________________________
VP1GeoFlags::SubSystemFlag VolumeHandle::subsystem() const
{
  return m_d->commondata->subSystemFlag();
}

//____________________________________________________________________
VolumeHandle::Imp::VolState VolumeHandle::Imp::getChildStates(const VolumeHandle* theclass)
{
  VolState vs;
  if (theclass->m_state!=VP1GeoFlags::EXPANDED||theclass->m_nchildren==0||!theclass->childrenAreInitialised())
    return vs;

  VolumeHandleListConstItr it, itE = theclass->m_children.end();
  for (it = theclass->m_children.begin(); it!=itE; ++it) {
    if ((*it)->state()==VP1GeoFlags::ZAPPED)
      vs.m_zappedChildren.insert((*it)->hashID());
    else if ((*it)->state()==VP1GeoFlags::EXPANDED)
      vs.m_expandedChildren.insert((*it)->hashID(),(*it)->m_d->getChildStates(*it));
  }
  return vs;
}

//____________________________________________________________________
void VolumeHandle::Imp::applyChildStates(const VolState& vs,VolumeHandle*theclass)
{
  bool hasExpandedChildren = !vs.m_expandedChildren.isEmpty();
  bool hasZappedChildren = !vs.m_zappedChildren.isEmpty();
  if (!hasExpandedChildren&&!hasZappedChildren)
    return;

  QMap<quint32,VolState>::const_iterator expItr, expItrEnd = vs.m_expandedChildren.end();
  QSet<quint32>::const_iterator zapItr, zapItrEnd = vs.m_zappedChildren.end();

  theclass->initialiseChildren();
  VolumeHandleListConstItr it, itE = theclass->m_children.end();
  for (it = theclass->m_children.begin(); it!=itE; ++it) {
    quint32 id = (*it)->hashID();
    zapItr = vs.m_zappedChildren.find(id);
    if (zapItr!=zapItrEnd) {
      (*it)->setState(VP1GeoFlags::ZAPPED);
      continue;
    }
    expItr = vs.m_expandedChildren.find(id);
    if (expItr!=expItrEnd) {
      (*it)->setState(VP1GeoFlags::ZAPPED);
      applyChildStates(expItr.value(),*it);
      (*it)->setState(VP1GeoFlags::EXPANDED);
    }
  }
}


//____________________________________________________________________
bool VolumeHandle::Imp::hasNonStandardShapeChildren(const SoGroup*g)
{
  const int n(g->getNumChildren());
  for (int i=0; i < n; ++i) {
    const SoNode*c = g->getChild(i);
    if (c->getTypeId().isDerivedFrom(SoShape::getClassTypeId())) {
      if (c->getTypeId().isDerivedFrom(SoPcons::getClassTypeId())
	  ||c->getTypeId().isDerivedFrom(SoPolyhedron::getClassTypeId())
	  ||c->getTypeId().isDerivedFrom(SoTransparency::getClassTypeId()))
	return true;
    } else if (c->getTypeId().isDerivedFrom(SoGroup::getClassTypeId())) {
      if (hasNonStandardShapeChildren(static_cast<const SoGroup*>(c)))
	return true;
    }
  }
  return false;
}

//____________________________________________________________________
bool VolumeHandle::isInitialisedAndHasNonStandardShape() const
{
  VP1HEPVisUtils::initAllCustomClasses();
  return m_d->nodesep ? Imp::hasNonStandardShapeChildren(m_d->nodesep) : false;
}




//____________________________________________________________________
bool VolumeHandle::isPositiveZ() const
{
  SbVec3f dir;
  m_d->accumTrans.multDirMatrix(SbVec3f(0,0,1),dir);
  float x,y,z;
  dir.getValue(x,y,z);
  return z>0;
}

