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

#include <nlohmann/json.hpp>

#include <map>
#include <iostream>
#include <fstream>
#include <unistd.h>


using json = nlohmann::json;

class GXHitDisplaySystem::Imp {
public:
  Imp(GXHitDisplaySystem*tc) : theclass(tc),
			       controller(0),
			       switch0(nullptr)
  {}
  GXHitDisplaySystem *theclass{nullptr};
  HitDisplaySysController * controller{nullptr};
  SoSwitch                * switch0{nullptr};
  SoDrawStyle             * drawStyle{nullptr};
  SoCoordinate3           * coords{nullptr};
  SoPointSet              * pointSet{nullptr};

    static SbColor4f color4f(const QColor& col) {
    return SbColor4f(std::max<float>(0.0f,std::min<float>(1.0f,col.redF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.greenF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.blueF())),
		     1.0);
  }

  json j;
};

namespace hitdisp {

    struct hit {
        int         eventID;
        double      x,y,z;
    };

    void to_json(json& j, const hit& p) {
        j = json{{"Event ID", p.eventID},{"x", p.x},{"y", p.y},{"z", p.z} };
    }

    void from_json(const json& j, hit& p) {
      j.at("Event ID").get_to(p.eventID);
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
        j.at("z").get_to(p.z);

    }
} // namespace hitdisp




//_____________________________________________________________________________________
GXHitDisplaySystem::GXHitDisplaySystem()
  : IVP13DSystemSimple("Hit Display",
		       "System providing visualizaton of hits",
		       "boudreau@pitt.edu"), m_d(new Imp(this))
{
}


//_____________________________________________________________________________________
GXHitDisplaySystem::~GXHitDisplaySystem()
{
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
				      tr("Hit input files (*.json)"),0,QFileDialog::DontUseNativeDialog);

  if (path!="") {
    m_d->switch0->removeAllChildren();
    std::ifstream i(path.toStdString());
    m_d->j=json::parse(i);
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
  std::cout << "Next Event" << std::endl;
  if (m_d->pointSet) m_d->switch0->removeChild(m_d->pointSet);
  if (m_d->coords)   m_d->switch0->removeChild(m_d->coords);
  
  m_d->coords = new SoCoordinate3;
  m_d->pointSet = new SoPointSet;
  m_d->switch0->addChild(m_d->coords);
  m_d->switch0->addChild(m_d->pointSet);
  
 
  try {
      unsigned int   counter=0;
      static int lastEventID=-1;
      int eventID           =-1;
      for (const auto& element : m_d->j["Events"]){
	int newEventID=element["Event ID"];
	if (newEventID<lastEventID) continue;
	if (eventID==-1) {
	  eventID=newEventID;
	}
	else if (eventID!=newEventID){
	  lastEventID=newEventID;
	  break;
	}
	lastEventID=eventID;
        
	m_d->coords->point.set1Value(counter++,element["x"], element["y"], element["z"]);
      }

      m_d->pointSet->numPoints=counter;
    }
    catch (std::exception & e) {
      std::cout << e.what() << std::endl;
     }

}
