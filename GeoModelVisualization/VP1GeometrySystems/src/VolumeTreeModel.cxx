/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#include "VP1GeometrySystems/VolumeTreeModel.h"

#include "GeoModelKernel/GeoMaterial.h"

#include <cassert>
#include <iostream>
#include <QColor>

//////////////////////////////////////////////////////////////////////
//NB: Since the QModelIndices uses void pointers, the VolumeHandle,
//SectionInfo and SubSystem classes must derive from the same
//base class before we know which class we can cast to.
//
//In order to not add any unnecessary overhead to VolumeHandle (which
//has a copy of the entire GeoModel tree), we let SectionInfo and
//SubSystem both inherit from VolumeHandle, and we use the
//m_childNumber field (which is always non-negative for actual
//VolumeHandles) of VolumeHandle to indicate the type:
//
// -2: SectionInfo
// -1: SubSystem
// 0+: Regular VolumeHandle
//
// This is taken care of by the constructors of the two derived
// classes, and four helper methods below makes the conversions a
// breeze (even though its a bit hackish behind the scenes)
//////////////////////////////////////////////////////////////////////////////

//____________________________________________________________________
class VolumeTreeModel::Imp {
public:

  //Class for the dynamic information of a given subsystem:
  class SectionInfo;
  class SubSystem: public VolumeHandle {
  public:
    SubSystem(SectionInfo *si,VP1GeoFlags::SubSystemFlag sf)
      : VolumeHandle(0,0, PVConstLink(), -1),
	section(si), subsysflag(sf) {}
    ~SubSystem() {
      VolumeHandle::VolumeHandleListItr volItr, volItrE(volhandlelist.end());
      for (volItr = volhandlelist.begin();volItr!=volItrE;++volItr)
	delete (*volItr);//This is where volume handles are deleted
    }
    //
    SectionInfo * section;
    VP1GeoFlags::SubSystemFlag subsysflag;
    VolumeHandle::VolumeHandleList volhandlelist;
    QString name;
  };
  //lists/maps for the added subsystems:
  std::map<VP1GeoFlags::SubSystemFlag,SubSystem*> flag2subsystems;
  //Map to quickly find subsystem from volumehandle. Only contains volumehandles from enabled subsystems:
  std::map<VolumeHandle*,SubSystem*> volhandle2subsystem;

  //Class for the dynamic information about sections and their daughter subsystems:
  class SectionInfo: public VolumeHandle {
  public:
    SectionInfo(): VolumeHandle(0,0, PVConstLink(), -2) {}
    //
    QList<SubSystem*> enabledSubSystems;
    QList<SubSystem*> disabledSubSystems;
  };
  //Lists of these sections:
  SectionInfo *theSection;
  SectionInfo *activeSection;

  //Convenience methods for dealing with the void pointers from the QModelIndices:
  static VolumeHandle* handlePointer(const QModelIndex& idx) { return static_cast<VolumeHandle*>(idx.internalPointer()); }
  static bool isSectionInfoPointer(VolumeHandle* handle) { return handle->childNumber()==-2; }
  static bool isSubSystemPointer(VolumeHandle* handle) { return handle->childNumber()==-1; }
  static bool isRegularVolumeHandle(VolumeHandle* handle) { return handle->childNumber()>=0; }
  static SectionInfo * sectionInfoPointer (VolumeHandle* handle) { return handle->childNumber()==-2 ? static_cast<SectionInfo*>(handle) : 0; }
  static SubSystem * subSystemPointer (VolumeHandle* handle) { return handle->childNumber()==-1 ? static_cast<SubSystem*>(handle) : 0; }
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////

};

//____________________________________________________________________
VolumeTreeModel::VolumeTreeModel( QObject * parent )
  : QAbstractItemModel(parent), m_d(new Imp())
{
  m_d->theSection=nullptr;
  m_d->activeSection=nullptr;
}

//____________________________________________________________________
void VolumeTreeModel::cleanup()
{
  //This is where we delete all SectionInfo/SubSystem pointers (and thus also all VolumeHandles):
  std::map<VP1GeoFlags::SubSystemFlag,Imp::SubSystem*>::iterator it, itE = m_d->flag2subsystems.end();
  for (it = m_d->flag2subsystems.begin();it!=itE;++it)
    disableSubSystem(it->first);
  for (it = m_d->flag2subsystems.begin();it!=itE;++it)
    delete it->second;
  delete m_d->theSection;
}

//____________________________________________________________________
VolumeTreeModel::~VolumeTreeModel()
{
  delete m_d;
}

//____________________________________________________________________
void VolumeTreeModel::addSubSystem( VP1GeoFlags::SubSystemFlag flag,
				    const VolumeHandle::VolumeHandleList& roothandles )
{
  if (!m_d->theSection) {
    m_d->theSection = new Imp::SectionInfo;
  }

  //Create SubSystem instance for this subsystem and give it the roothandles:
  Imp::SubSystem * subsys = new Imp::SubSystem(m_d->theSection,flag);
  subsys->name=flag.c_str();
  subsys->volhandlelist = roothandles;

  //Add the subsystem pointer to the relevant maps:
  m_d->theSection->disabledSubSystems << subsys;
  m_d->flag2subsystems[flag]=subsys;
}

//____________________________________________________________________
void VolumeTreeModel::enableSubSystem(VP1GeoFlags::SubSystemFlag flag)
{
  beginResetModel(); // see: http://doc.qt.io/qt-5/qabstractitemmodel-obsolete.html

  //Check the subsystem was added previously:
  if (m_d->flag2subsystems.find(flag)==m_d->flag2subsystems.end()) {
    std::cout<<"VolumeTreeModel::enableSubSystem Error: System never added!"<<std::endl;
    return;
  }
  Imp::SubSystem * subsys = m_d->flag2subsystems[flag];
  //Find the appropriate section:
  Imp::SectionInfo* section(0);
  if (m_d->theSection) {
    if (m_d->theSection->enabledSubSystems.contains(subsys)) {
      //It is already enabled
      assert(!m_d->theSection->disabledSubSystems.contains(subsys));
      return;
    }
    if (m_d->theSection->disabledSubSystems.contains(subsys)) {
      assert(!m_d->theSection->enabledSubSystems.contains(subsys));
      section=m_d->theSection;
    }
  }
  assert(section);
  if (!section) {
    std::cout<<"VolumeTreeModel::enableSubSystem Error: Did not find section of subsystem!."<<std::endl;
    return;
  }
  //Move the subsystem from the disabled to the enabled list:
  section->enabledSubSystems << subsys;//Fixme: Ordering.
  section->disabledSubSystems.removeAll(subsys);
  //If the newly added subsystem is the only enabled subsystem, the section needs to be enabled as well:
  if (section->enabledSubSystems.count()==1) {
    //assert(!m_d->activeSections.contains(section));
    m_d->activeSection = section;//Fixme: Ordering.
  }
  //Put volume handle pointers into quick subsystem access map:
  foreach (VolumeHandle* volhandle, subsys->volhandlelist ) {
    m_d->volhandle2subsystem[volhandle] = subsys;
  }

  endResetModel();

}

//____________________________________________________________________
void VolumeTreeModel::disableSubSystem(VP1GeoFlags::SubSystemFlag flag)
{
	beginResetModel(); // see: http://doc.qt.io/qt-5/qabstractitemmodel-obsolete.html

  //If it was not even added previously we can just return:
  if (m_d->flag2subsystems.find(flag)==m_d->flag2subsystems.end())
    return;

  Imp::SubSystem * subsys = m_d->flag2subsystems[flag];
  //Find the appropriate section:
  Imp::SectionInfo* section(0);
  if (m_d->theSection) {
    if (m_d->theSection->disabledSubSystems.contains(subsys)) {
      //It is already disabled
      assert(!m_d->theSection->enabledSubSystems.contains(subsys));
      return;
    }
    if (m_d->theSection->enabledSubSystems.contains(subsys)) {
      assert(!m_d->theSection->disabledSubSystems.contains(subsys));
      section=m_d->theSection;
    }
  }
  assert(section);
  if (!section) {
    std::cout<<"VolumeTreeModel::disableSubSystem Error: Did not find section of subsystem!."<<std::endl;
    return;
  }

  //Move the subsystem from the enabled to the disabled list:
  section->disabledSubSystems << subsys;
  section->enabledSubSystems.removeAll(subsys);
  //If the newly disabled subsystem was the only enabled subsystem, the section needs to be disabled as well:
  if (section->enabledSubSystems.count()==0) {
    //assert(m_d->activeSections.contains(section));
    m_d->activeSection=nullptr;
  }

  //Remove volume handle pointers from quick subsystem access map:
  foreach (VolumeHandle* volhandle, subsys->volhandlelist ) {
    Q_ASSERT(m_d->volhandle2subsystem.find(volhandle)!=m_d->volhandle2subsystem.end());
    m_d->volhandle2subsystem.erase(m_d->volhandle2subsystem.find(volhandle));
  }

  endResetModel();
//  reset();//Fixme: use proper insert rows/colums/etc. instead!
}

//____________________________________________________________________
void VolumeTreeModel::getRootHandles(std::vector<std::pair<VolumeHandle::VolumeHandleListItr,VolumeHandle::VolumeHandleListItr> >& out) const
{
  out.clear();
  out.reserve(m_d->flag2subsystems.size());
  std::map<VP1GeoFlags::SubSystemFlag,Imp::SubSystem*>::iterator it, itE = m_d->flag2subsystems.end();
  for (it = m_d->flag2subsystems.begin();it!=itE;++it)
    out.push_back(std::pair<VolumeHandle::VolumeHandleListItr,VolumeHandle::VolumeHandleListItr>
		  (it->second->volhandlelist.begin(),it->second->volhandlelist.end()));

}

//____________________________________________________________________
QModelIndex VolumeTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  //Check that row and column are in allowed ranges (positive and within row/column count of parent):
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  if (!parent.isValid()) {
    //We must return the index of a section label:
    //Q_ASSERT(row<m_d->activeSections.count());
    return createIndex(row, column, m_d->activeSection);
  }

  VolumeHandle * parentHandle = Imp::handlePointer(parent);

  if (Imp::isRegularVolumeHandle(parentHandle)) {
    if (!parentHandle->childrenAreInitialised())
      parentHandle->initialiseChildren();//Fixme: It seems that it is occasionally necessary to do this. Why?? Why not fetchMore??
    VolumeHandle * childHandle = parentHandle->child(row);
    Q_ASSERT(childHandle);
    return createIndex(row, column, childHandle);
  }

  if (Imp::isSubSystemPointer(parentHandle)) {
    //Return index of top-level volume:
    Q_ASSERT(unsigned(row)<Imp::subSystemPointer(parentHandle)->volhandlelist.size());
    return createIndex(row, column, Imp::subSystemPointer(parentHandle)->volhandlelist.at(row));
  }

  //Must be SectionInfo:
  Q_ASSERT(Imp::isSectionInfoPointer(parentHandle));
  Q_ASSERT(row<Imp::sectionInfoPointer(parentHandle)->enabledSubSystems.count());
  return createIndex(row, column, Imp::sectionInfoPointer(parentHandle)->enabledSubSystems.at(row));
}

//____________________________________________________________________
QModelIndex VolumeTreeModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();

  //See if we have a volumeHandle as index:
  VolumeHandle *childHandle = Imp::handlePointer(index);

  if (Imp::isRegularVolumeHandle(childHandle)) {
    VolumeHandle *parentHandle = childHandle->parent();
    if (parentHandle)
      return createIndex(parentHandle->childNumber(), 0, parentHandle);

    //childHandle has 0 parent pointer => parent must be a subsystem label:
    Q_ASSERT(m_d->volhandle2subsystem.find(childHandle)!=m_d->volhandle2subsystem.end());
    Imp::SubSystem * subsys = m_d->volhandle2subsystem[childHandle];
    Q_ASSERT(subsys);
    Q_ASSERT(subsys->section->enabledSubSystems.contains(subsys));
    return createIndex(subsys->section->enabledSubSystems.indexOf(subsys), 0, subsys);
  }


  if (Imp::isSubSystemPointer(childHandle)) {
      //Index is a SubSystem => parent must be a section label:
      //Q_ASSERT(m_d->activeSections.contains(Imp::subSystemPointer(childHandle)->section));
      return createIndex(0, 0, Imp::subSystemPointer(childHandle)->section);
  }

  //Must be SectionInfo => parent is root (i.e. invalid):
  Q_ASSERT(Imp::isSectionInfoPointer(childHandle));
  return QModelIndex();
}

//____________________________________________________________________
int VolumeTreeModel::rowCount(const QModelIndex& parent) const
{
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid()) return m_d->activeSection ? 1 : 0;

  VolumeHandle * parentHandle = Imp::handlePointer(parent);

  if (Imp::isRegularVolumeHandle(parentHandle)) {
    return parentHandle->nChildren();
  }

  if (Imp::isSubSystemPointer(parentHandle)) {
    return Imp::subSystemPointer(parentHandle)->volhandlelist.size();
  }

  //Must be SectionInfo pointer:
  Q_ASSERT(Imp::isSectionInfoPointer(parentHandle));
  return Imp::sectionInfoPointer(parentHandle)->enabledSubSystems.count();
}

//____________________________________________________________________
QVariant VolumeTreeModel::data(const QModelIndex& index, int role) const
{
  if ((role!=Qt::DisplayRole&&role!=Qt::TextColorRole)||!index.isValid())
    return QVariant();

  VolumeHandle *volumeHandle = Imp::handlePointer(index);
  if (Imp::isRegularVolumeHandle(volumeHandle)) {
    if (role==Qt::TextColorRole) {
      if (volumeHandle->isAttached())
	return QVariant();
      else
	return QColor::fromRgbF( 0.5, 0.5, 0.5 );
    }
    
    //DisplayRole:
    
    QString volState = "e";
    if(volumeHandle->state()==VP1GeoFlags::CONTRACTED) volState = "c";
    else if(volumeHandle->state()==VP1GeoFlags::ZAPPED) volState = "z";

    if (volumeHandle->nChildren()>0)
      return volumeHandle->getName() + " (" + QString::fromStdString(volumeHandle->geoMaterial()->getName()) + ") [" + QString::number(volumeHandle->nChildren())+"] ("+ volState + ")" ;
    else
      return volumeHandle->getName() + " (" + QString::fromStdString(volumeHandle->geoMaterial()->getName()) + ") ("+ volState + ")" ;
  }

  if (role==Qt::TextColorRole)
    return QVariant();

  if (Imp::isSubSystemPointer(volumeHandle))
    return Imp::subSystemPointer(volumeHandle)->name;

  Q_ASSERT(Imp::isSectionInfoPointer(volumeHandle));
  return "GEOMETRY";
}


//____________________________________________________________________
Qt::ItemFlags VolumeTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  if (Imp::isRegularVolumeHandle(Imp::handlePointer(index)))
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  else
    return Qt::ItemIsEnabled;
}

//____________________________________________________________________
QVariant VolumeTreeModel::headerData(int /*section*/, Qt::Orientation /*orientation*/,int /*role*/) const
{
  return QVariant();
}

//____________________________________________________________________
bool VolumeTreeModel::canFetchMore( const QModelIndex & parent ) const
{
  if (!parent.isValid())
    return false;

  VolumeHandle * parentHandle = Imp::handlePointer(parent);

  if (Imp::isRegularVolumeHandle(parentHandle)&&!parentHandle->childrenAreInitialised())
    return true;

  return false;
}

//____________________________________________________________________
void VolumeTreeModel::fetchMore( const QModelIndex & parent )
{
  if (!parent.isValid())
    return;//should probably never happen

  VolumeHandle* parentHandle = Imp::handlePointer(parent);

  if (Imp::isRegularVolumeHandle(parentHandle)&&!parentHandle->childrenAreInitialised()) {
    //     beginInsertRows(parent,0,int(parentHandle->nChildren())-1);
    parentHandle->initialiseChildren();
    layoutChanged();//fixme??
    //     endInsertRows();
    return;
  }
}

//____________________________________________________________________
bool VolumeTreeModel::hasChildren ( const QModelIndex & parent ) const
{
  return rowCount(parent)>0;//Our rowCount is relatively fast (no looping to count).
}
