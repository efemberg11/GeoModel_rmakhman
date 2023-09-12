/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Implementation of class GXHitDisplaySystem                         //
//                                                                     //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>              //
//                                                                     //
//  Initial version: July 2007                                         //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "GXHitDisplaySystems/GXHitDisplaySystem.h"
#include "GXHitDisplaySystems/HitDisplaySysController.h"
#include "H5Cpp.h"
#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1Deserialise.h"
#include "VP1Base/VP1Msg.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/SbVec3f.h>

#include <QFileDialog>
#include <QMessageBox>
#include <map>
#include <iostream>
#include <fstream>
#include <unistd.h>


using namespace H5;



struct Hit{
  float X;
  float Y;
  float Z;
  int   ID;
};




class GXHitDisplaySystem::Imp {
public:
  Imp(GXHitDisplaySystem*tc) : theclass(tc)
			 
  {}
  GXHitDisplaySystem *theclass{nullptr};
  HitDisplaySysController * controller{nullptr};
  SoSwitch                * switch0{nullptr};
  SoDrawStyle             * drawStyle{nullptr};
  SoCoordinate3           * coords{nullptr};
  SoPointSet              * pointSet{nullptr};

  
  CompType datatype{sizeof(Hit)};
  H5File   *file{nullptr};
  std::vector<Hit> hit;



  
    static SbColor4f color4f(const QColor& col) {
    return SbColor4f(std::max<float>(0.0f,std::min<float>(1.0f,col.redF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.greenF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.blueF())),
		     1.0);
  }

};

extern "C" herr_t file_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void * md) {

  GXHitDisplaySystem::Imp * m_d = (GXHitDisplaySystem::Imp *) md;
  
  if (!m_d->file)      return -1;
  if (strlen(name)==0) return -1;

  
  const std::string datasetName=name;
  DataSet dataset=m_d->file->openDataSet(datasetName);
  size_t length=dataset.getStorageSize()/sizeof(Hit);
  m_d->hit.resize(length);  
  
  
  dataset.read(m_d->hit.data(),m_d->datatype);
  
  
  return 1;
}





//_____________________________________________________________________________________
GXHitDisplaySystem::GXHitDisplaySystem()
  : IVP13DSystemSimple("Hit Display",
		       "System providing visualizaton of hits",
		       "boudreau@pitt.edu"), m_d(new Imp(this))
{
  m_d->datatype.insertMember("X", HOFFSET(Hit,X),PredType::NATIVE_FLOAT);
  m_d->datatype.insertMember("Y", HOFFSET(Hit,Y),PredType::NATIVE_FLOAT);
  m_d->datatype.insertMember("Z", HOFFSET(Hit,Z),PredType::NATIVE_FLOAT);
  m_d->datatype.insertMember("ID", HOFFSET(Hit,ID),PredType::NATIVE_INT);

}


//_____________________________________________________________________________________
GXHitDisplaySystem::~GXHitDisplaySystem()
{
  delete m_d->file;
  delete m_d;
}

//_____________________________________________________________________________________
QWidget * GXHitDisplaySystem::buildController()
{
  m_d->controller = new HitDisplaySysController(this);
  connect(m_d->controller,
          SIGNAL(inputFileChanged()),
	  this,
	  SLOT(selectInputFile()));

  return m_d->controller;
}

//_____________________________________________________________________________________
void GXHitDisplaySystem::buildEventSceneGraph(StoreGateSvc*, SoSeparator *)
{
}

//_____________________________________________________________________________________
void GXHitDisplaySystem::buildPermanentSceneGraph(StoreGateSvc* /*detstore*/, SoSeparator *root)
{
  //No guidelines are pickable:
  SoPickStyle *pickStyle = new SoPickStyle;
  pickStyle->style=SoPickStyle::UNPICKABLE;
  root->addChild(pickStyle);

  m_d->drawStyle=new SoDrawStyle;

  m_d->drawStyle->pointSize.setValue(3);
  root->addChild(m_d->drawStyle);

  SoSeparator *s0=new SoSeparator;
  SoSeparator *s1=new SoSeparator;
  SoSeparator *s2=new SoSeparator;
  SoSeparator *s3=new SoSeparator;

  root->addChild(s0);
  root->addChild(s1);
  root->addChild(s2);
  root->addChild(s3);


  SoBaseColor *red=new SoBaseColor;
  SoBaseColor *green=new SoBaseColor;
  SoBaseColor *blue=new SoBaseColor;
  SoBaseColor *folly=new SoBaseColor;

  red->rgb.setValue(1,0,0);
  green->rgb.setValue(0,1,0);
  blue->rgb.setValue(0,0,1);
  folly->rgb.setValue(1.00,0,0.37);

  s0->addChild(red);
  s1->addChild(green);
  s2->addChild(blue);
  s3->addChild(folly);

  m_d->switch0=new SoSwitch;

  m_d->switch0->whichChild=SO_SWITCH_ALL;


  s0->addChild(m_d->switch0);

  connect(m_d->controller,SIGNAL(showHitDisplays1Changed(bool)),this,SLOT(showHitDisplay1(bool)));
  connect(m_d->controller,SIGNAL(setPointSizeChanged(int)), this, SLOT(setPointSize(int)));
  connect(m_d->controller,SIGNAL(nextEvent()), this, SLOT(nextEvent()));
}


    
//_____________________________________________________________________________________
void GXHitDisplaySystem::systemuncreate()
{

  m_d->controller=0;
}


//_____________________________________________________________________________________
QByteArray GXHitDisplaySystem::saveState() {

  ensureBuildController();

  VP1Serialise serialise(1/*version*/,this);
  serialise.save(IVP13DSystemSimple::saveState());
  serialise.save(m_d->controller->saveSettings());
  serialise.disableUnsavedChecks();//We do the testing in the controller
  return serialise.result();
}

//_____________________________________________________________________________________
void GXHitDisplaySystem::restoreFromState(QByteArray ba) {

  VP1Deserialise state(ba,this);
  ensureBuildController();

  IVP13DSystemSimple::restoreFromState(state.restoreByteArray());
  m_d->controller->restoreSettings(state.restoreByteArray());
  state.disableUnrestoredChecks();//We do the testing in the controller
}

void GXHitDisplaySystem::selectInputFile() {


  QString path;
  char buffer[1024];
  char *wd=getcwd(buffer,1024);
  path = QFileDialog::getOpenFileName(nullptr, tr("Open Input File"),
				      wd,
				      tr("Hit input files (*.h5)"),0,QFileDialog::DontUseNativeDialog);

  if (path!="") {
    m_d->switch0->removeAllChildren();

    delete m_d->file;
    m_d->file=new H5File(path.toStdString(), H5F_ACC_RDONLY);

    nextEvent();
  }

}

void GXHitDisplaySystem::setPointSize(int size) {
  m_d->drawStyle->pointSize.setValue(size);
}

 
void GXHitDisplaySystem::showHitDisplay1(bool flag) {
  m_d->switch0->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;
}

void GXHitDisplaySystem::nextEvent() {
  

  
  if (m_d->pointSet) m_d->switch0->removeChild(m_d->pointSet);
  if (m_d->coords)   m_d->switch0->removeChild(m_d->coords);

 
  static hsize_t count=0;
  if (count==m_d->file->getNumObjs()) {
    QMessageBox msgBox;
    msgBox.setText("Last event reached.  Reset stream to beginning.");
    msgBox.exec();
    count=0;
  }
  H5Literate(m_d->file->getId(), H5_INDEX_NAME, H5_ITER_INC, &count, file_info, m_d);



  
  m_d->coords = new SoCoordinate3;
  m_d->pointSet = new SoPointSet;
  m_d->switch0->addChild(m_d->coords);
  m_d->switch0->addChild(m_d->pointSet);

  size_t counter(0);
  for (const Hit & h: m_d->hit) m_d->coords->point.set1Value(counter++, h.X, h.Y, h.Z);
  m_d->pointSet->numPoints=m_d->hit.size();
  m_d->hit.clear();
}
