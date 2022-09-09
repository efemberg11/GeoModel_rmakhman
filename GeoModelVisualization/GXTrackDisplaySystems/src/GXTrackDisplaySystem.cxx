/*
  Copyright (C) 2002-2012 CERN for the benefit of the ATLAS collaboration
*/


/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Implementation of class GXTrackDisplaySystem                       //
//                                                                     //
//  Based on implementation of class GXHitDisplaySystem                //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "GXTrackDisplaySystems/GXTrackDisplaySystem.h"
#include "GXTrackDisplaySystems/TrackDisplaySysController.h"
#include "H5Cpp.h"
#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1Deserialise.h"
#include "VP1Base/VP1Msg.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoBaseColor.h>

#include <QFileDialog>
#include <QMessageBox>
#include <map>
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace H5;
using namespace std;

struct stepData
{
  float x;
  float y;
  float z;
};

class GXTrackDisplaySystem::Imp
{
public:
  Imp(GXTrackDisplaySystem *tc) : theclass(tc) {}

  GXTrackDisplaySystem *theclass{nullptr};
  TrackDisplaySysController *controller{nullptr};
  SoSwitch *switch0{nullptr};
  SoDrawStyle *drawStyle{nullptr};
  SoCoordinate3 *coords{nullptr};
  SoPointSet *pointSet{nullptr};
  SoLineSet *lineSet{nullptr};

  vector<int> trackNums;

  CompType datatype{sizeof(stepData)};
  H5File *file{nullptr};

  vector<int> pdgs;
  vector<vector<stepData>> trksStepsVec;

  vector<SoSeparator*> tracksSoSwitches;

  void readEventTracks(int);
};

void GXTrackDisplaySystem::Imp::readEventTracks(int eventNum)
{
  pdgs.clear();
  trksStepsVec.clear();

  std::string pdgDataSetName = "event" + std::to_string(eventNum) + "PDGs";

  DataSet *pdgDataSet = new DataSet(file->openDataSet(pdgDataSetName));
  size_t buffSize = pdgDataSet->getStorageSize() / sizeof(int);
  pdgs.resize(buffSize);
  pdgDataSet->read(pdgs.data(), PredType::NATIVE_INT);

  trksStepsVec.resize(buffSize);

  for (int trkNum = 0; trkNum < trackNums[eventNum]; ++trkNum)
  {
    vector<stepData> &steps = trksStepsVec[trkNum];

    std::string dataSetName = "event" + std::to_string(eventNum) +
                              "-trk" + std::to_string(trkNum+1);

    DataSet *datasetE1T1 = new DataSet(file->openDataSet(dataSetName));
    /*size_t*/ buffSize = datasetE1T1->getStorageSize() / sizeof(stepData);
    steps.resize(buffSize);
    datasetE1T1->read(steps.data(),  datatype);

    delete datasetE1T1;
  }

  delete pdgDataSet;

}

//_____________________________________________________________________________________
GXTrackDisplaySystem::GXTrackDisplaySystem()
    : IVP13DSystemSimple("Track Display",
                         "System providing visualizaton of tracks",
                         "denys.klekots@cern.ch, denys.klekots@gmail.com"),
      m_d(new Imp(this))
{
  m_d->datatype.insertMember("x", HOFFSET(stepData, x), PredType::NATIVE_FLOAT);
  m_d->datatype.insertMember("y", HOFFSET(stepData, y), PredType::NATIVE_FLOAT);
  m_d->datatype.insertMember("z", HOFFSET(stepData, z), PredType::NATIVE_FLOAT);
}

//_____________________________________________________________________________________
GXTrackDisplaySystem::~GXTrackDisplaySystem()
{
  delete m_d->file;
  delete m_d;
}

//_____________________________________________________________________________________
QWidget *GXTrackDisplaySystem::buildController()
{
  m_d->controller = new TrackDisplaySysController(this);
  connect(m_d->controller,
          SIGNAL(inputFileChanged()),
          this,
          SLOT(selectInputFile()));

  return m_d->controller;
}

//_____________________________________________________________________________________
void GXTrackDisplaySystem::buildEventSceneGraph(StoreGateSvc *, SoSeparator *)
{
}

//_____________________________________________________________________________________
void GXTrackDisplaySystem::buildPermanentSceneGraph(StoreGateSvc * /*detstore*/, SoSeparator *root)
{
  // No guidelines are pickable:
  SoPickStyle *pickStyle = new SoPickStyle;
  pickStyle->style = SoPickStyle::UNPICKABLE;
  root->addChild(pickStyle);

  m_d->drawStyle = new SoDrawStyle;

  m_d->drawStyle->lineWidth.setValue(3);
  root->addChild(m_d->drawStyle);

  SoSeparator *s0 = new SoSeparator;

  root->addChild(s0);

  m_d->switch0 = new SoSwitch;

  m_d->switch0->whichChild = SO_SWITCH_ALL;

  s0->addChild(m_d->switch0);

  connect(m_d->controller, SIGNAL(showTrackDisplays1Changed(bool)), this, SLOT(showTrackDisplay1(bool)));
  connect(m_d->controller, SIGNAL(setLineWidthChanged(int)), this, SLOT(setLineWidth(int)));
  connect(m_d->controller, SIGNAL(nextEvent()), this, SLOT(nextEvent()));
}

//_____________________________________________________________________________________
void GXTrackDisplaySystem::systemuncreate()
{

  m_d->controller = 0;
}

//_____________________________________________________________________________________
QByteArray GXTrackDisplaySystem::saveState()
{

  ensureBuildController();

  VP1Serialise serialise(1 /*version*/, this);
  serialise.save(IVP13DSystemSimple::saveState());
  serialise.save(m_d->controller->saveSettings());
  serialise.disableUnsavedChecks(); // We do the testing in the controller
  return serialise.result();
}

//_____________________________________________________________________________________
void GXTrackDisplaySystem::restoreFromState(QByteArray ba)
{

  VP1Deserialise state(ba, this);
  ensureBuildController();

  IVP13DSystemSimple::restoreFromState(state.restoreByteArray());
  m_d->controller->restoreSettings(state.restoreByteArray());
  state.disableUnrestoredChecks(); // We do the testing in the controller
}

void GXTrackDisplaySystem::selectInputFile()
{

  QString path;
  char buffer[1024];
  char *wd = getcwd(buffer, 1024);
  path = QFileDialog::getOpenFileName(nullptr, tr("Open Input File"), wd,
                            tr("Hit input files (*.h5)"), 0, QFileDialog::DontUseNativeDialog);

  if (path != "")
  {
    m_d->switch0->removeAllChildren();

    delete m_d->file;
    m_d->file = new H5File(path.toStdString(), H5F_ACC_RDONLY);

    DataSet *eventNumsDataset = new DataSet(m_d->file->openDataSet("eventNums"));
    size_t buffSize = eventNumsDataset->getStorageSize() / sizeof(int);
    m_d->trackNums.resize(buffSize);
    eventNumsDataset->read(m_d->trackNums.data(), PredType::NATIVE_INT);

    nextEvent();

    delete eventNumsDataset;
  }
}

void GXTrackDisplaySystem::setLineWidth(int size)
{
  m_d->drawStyle->lineWidth.setValue(size);
}

void GXTrackDisplaySystem::showTrackDisplay1(bool flag)
{
  m_d->switch0->whichChild = flag ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

SoBaseColor* getColor(int pdg)
{
  SoBaseColor *returnColor = new SoBaseColor;

  switch (pdg)
  {
  case 2212: // proton
    returnColor->rgb.setValue(1,0,0); //red
    break;
  case 2112: // neutron
    returnColor->rgb.setValue(0.75,0.75,0.75); //gray
    break;
  case 11: // electron
    returnColor->rgb.setValue(0,0,1); //blue
    break;
  case -11: // positron
    returnColor->rgb.setValue(0.54,0.17,1); //violet
    break;
  case 22: // gamma
    returnColor->rgb.setValue(0,1,0); //green
    break;
  case -22: // optical photon
    returnColor->rgb.setValue(0,1,0); //green
    break;
  case 211: // charget pions
    returnColor->rgb.setValue(1,0,1); //magenta
    break;
  case -211: // charget pions
    returnColor->rgb.setValue(1,0,1); //magenta
    break;
  case 111: // neutral pions
    returnColor->rgb.setValue(1,0,1); //magenta
    break;
  case 13: // muon
    returnColor->rgb.setValue(1,1,0); //yellow
    break;
  case -13: // muon
    returnColor->rgb.setValue(1,1,0); //yellow
    break;
  case 321: // kaon+
    returnColor->rgb.setValue(0,1,1); //cyan
    break;
  case -321: // kaon-
    returnColor->rgb.setValue(0,1,1); //cyan
    break;
  default:
    returnColor->rgb.setValue(0,0,0); //black
  }

  return returnColor;
}

void GXTrackDisplaySystem::nextEvent()
{

  for(SoSeparator* el: m_d->tracksSoSwitches)
  {
    // el->removeAllChildren();
    m_d->switch0->removeChild(el);
  }

  static hsize_t count = 0;
  if (count == m_d->trackNums.size())
  {
    QMessageBox msgBox;
    msgBox.setText("Last event reached.  Reset stream to beginning.");
    msgBox.exec();
    count = 0;
  }
  m_d->readEventTracks(count++);


  m_d->tracksSoSwitches.resize(m_d->trksStepsVec.size());

  for (int trkNum = 0; trkNum < m_d->trksStepsVec.size(); ++trkNum)
  {
    SoCoordinate3 *tmpCoords = new SoCoordinate3;
    SoPointSet *tmpPointSet = new SoPointSet;
    SoLineSet *lineSet = new SoLineSet;

    m_d->tracksSoSwitches[trkNum] = new SoSeparator;

    m_d->tracksSoSwitches[trkNum]->addChild(tmpCoords);
    m_d->tracksSoSwitches[trkNum]->addChild(tmpPointSet);
    m_d->tracksSoSwitches[trkNum]->addChild(lineSet);

    m_d->switch0->addChild(getColor(m_d->pdgs[trkNum]));

    for(int stepNum = 0; stepNum < m_d->trksStepsVec[trkNum].size(); ++stepNum)
    {
      float &x = m_d->trksStepsVec[trkNum][stepNum].x;
      float &y = m_d->trksStepsVec[trkNum][stepNum].y;
      float &z = m_d->trksStepsVec[trkNum][stepNum].z;

      tmpCoords->point.set1Value(stepNum, x, y, z);
    }

    tmpPointSet->numPoints = m_d->trksStepsVec[trkNum].size();
    lineSet->numVertices = m_d->trksStepsVec[trkNum].size();

    m_d->switch0->addChild(m_d->tracksSoSwitches[trkNum]);
  }

}