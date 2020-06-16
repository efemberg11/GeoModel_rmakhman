/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Implementation of class GXClashPointSystem                         //
//                                                                     //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>              //
//                                                                     //
//  Initial version: July 2007                                         //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "GXClashPointSystems/GXClashPointSystem.h"
#include "GXClashPointSystems/ClashPointSysController.h"

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

class GXClashPointSystem::Imp {
public:
  Imp(GXClashPointSystem*tc) : theclass(tc),
			       controller(0),
			       switch0(nullptr),
			       switch1(nullptr),
			       switch2(nullptr),
                   switch3(nullptr)
  {}
  GXClashPointSystem *theclass;
  ClashPointSysController * controller;
  SoSwitch                * switch0;
  SoSwitch                * switch1;
  SoSwitch                * switch2;
  SoSwitch                * switch3;


    static SbColor4f color4f(const QColor& col) {
    return SbColor4f(std::max<float>(0.0f,std::min<float>(1.0f,col.redF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.greenF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.blueF())),
		     1.0);
  }


};

namespace clashdet {
    enum typeOfClash{ withMother=0, withSister, fullyEncapsSister, invalidSolid};
    // a simple struct to model a clash detection error
    struct clash {
        typeOfClash clashType;
        std::string volume1Name;
        int         volume1CopyNo;
        std::string volume1EntityType;
        std::string volume2Name;
        int         volume2CopyNo;
        std::string volume2EntityType;
        double      x,y,z;
        double      distance;
    };

    void to_json(json& j, const clash& p) {
        j = json{{"typeOfClash", p.clashType}, {"volume1Name", p.volume1Name}, {"volume1CopyNo", p.volume1CopyNo}, {"volume1EntityType", p.volume1EntityType},{"volume2Name", p.volume2Name},{"volume2CopyNo", p.volume2CopyNo}
, {"volume2EntityType", p.volume2EntityType},{"x", p.x},{"y", p.y},{"z", p.z},{"distance[mm]", p.distance} };
    }

    void from_json(const json& j, clash& p) {
        j.at("clashType").get_to(p.clashType);
        j.at("volume1Name").get_to(p.volume1Name);
        j.at("volume1CopyNo").get_to(p.volume1CopyNo);
        j.at("volume1EntityType").get_to(p.volume1EntityType);
        j.at("volume2Name").get_to(p.volume2Name);
        j.at("volume2CopyNo").get_to(p.volume2CopyNo);
        j.at("volume2EntityType").get_to(p.volume2EntityType);
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
        j.at("z").get_to(p.z);
        j.at("distance[mm]").get_to(p.distance);

    }
} // namespace clashdet




//_____________________________________________________________________________________
GXClashPointSystem::GXClashPointSystem()
  : IVP13DSystemSimple("Clash Points",
		       "System providing visualizaton of clash points",
		       "boudreau@pitt.edu"), m_d(new Imp(this))
{
}


//_____________________________________________________________________________________
GXClashPointSystem::~GXClashPointSystem()
{
  delete m_d;
}

//_____________________________________________________________________________________
QWidget * GXClashPointSystem::buildController()
{
  m_d->controller = new ClashPointSysController(this);
  connect(m_d->controller,
          SIGNAL(inputFileChanged()),
	  this,
	  SLOT(selectInputFile()));

  return m_d->controller;
}

//_____________________________________________________________________________________
void GXClashPointSystem::buildEventSceneGraph(StoreGateSvc*, SoSeparator *)
{
}

//_____________________________________________________________________________________
void GXClashPointSystem::buildPermanentSceneGraph(StoreGateSvc* /*detstore*/, SoSeparator *root)
{
  //No guidelines are pickable:
  SoPickStyle *pickStyle = new SoPickStyle;
  pickStyle->style=SoPickStyle::UNPICKABLE;
  root->addChild(pickStyle);

  SoDrawStyle *drawStyle=new SoDrawStyle;
  drawStyle->pointSize.setValue(3);
  root->addChild(drawStyle);

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
  m_d->switch1=new SoSwitch;
  m_d->switch2=new SoSwitch;
  m_d->switch3=new SoSwitch;

  m_d->switch0->whichChild=SO_SWITCH_ALL;
  m_d->switch1->whichChild=SO_SWITCH_ALL;
  m_d->switch2->whichChild=SO_SWITCH_ALL;
  m_d->switch3->whichChild=SO_SWITCH_ALL;


  s0->addChild(m_d->switch0);
  s1->addChild(m_d->switch1);
  s2->addChild(m_d->switch2);
  s3->addChild(m_d->switch3);

  connect(m_d->controller,SIGNAL(showClashPoints1Changed(bool)),this,SLOT(showClashPoint1(bool)));
  connect(m_d->controller,SIGNAL(showClashPoints2Changed(bool)),this,SLOT(showClashPoint2(bool)));
  connect(m_d->controller,SIGNAL(showClashPoints3Changed(bool)),this,SLOT(showClashPoint3(bool)));
  connect(m_d->controller,SIGNAL(showClashPoints4Changed(bool)),this,SLOT(showClashPoint4(bool)));

}

//_____________________________________________________________________________________
void GXClashPointSystem::systemuncreate()
{

  m_d->controller=0;
}


//_____________________________________________________________________________________
QByteArray GXClashPointSystem::saveState() {

  ensureBuildController();

  VP1Serialise serialise(1/*version*/,this);
  serialise.save(IVP13DSystemSimple::saveState());
  serialise.save(m_d->controller->saveSettings());
  serialise.disableUnsavedChecks();//We do the testing in the controller
  return serialise.result();
}

//_____________________________________________________________________________________
void GXClashPointSystem::restoreFromState(QByteArray ba) {

  VP1Deserialise state(ba,this);
  ensureBuildController();

  IVP13DSystemSimple::restoreFromState(state.restoreByteArray());
  m_d->controller->restoreSettings(state.restoreByteArray());
  state.disableUnrestoredChecks();//We do the testing in the controller
}

void GXClashPointSystem::selectInputFile() {


  QString path;
  char buffer[1024];
  char *wd=getcwd(buffer,1024);
  path = QFileDialog::getOpenFileName(nullptr, tr("Open Input File"),
				      wd,
				      tr("Clashpoint files (*.json)"),0,QFileDialog::DontUseNativeDialog);
  SoGroup *switches[]={m_d->switch0, m_d->switch1, m_d->switch2, m_d->switch3};

  if (path!="") {
    m_d->switch0->removeAllChildren();
    m_d->switch1->removeAllChildren();
    m_d->switch2->removeAllChildren();
    m_d->switch3->removeAllChildren();
    std::ifstream i(path.toStdString());
    auto j=json::parse(i);

    try {
      SoCoordinate3 *coords[]={new SoCoordinate3, new SoCoordinate3, new SoCoordinate3, new SoCoordinate3};
      unsigned int   counter[]={0,0,0,0};

      for (const auto& element : j["ClashesReport"]){
	    clashdet::typeOfClash type=element["typeOfClash"];
	    coords[type]->point.set1Value(counter[type]++,element["x"], element["y"], element["z"]);
      }
      for (int i=0;i<4;i++) {
	    switches[i]->addChild(coords[i]);
	    SoPointSet *pointSet=new SoPointSet;
	    pointSet->numPoints=counter[i];
	    switches[i]->addChild(pointSet);
      }
    }
    catch (std::exception & e) {
      std::cout << e.what() << std::endl;
     }

  }

}


void GXClashPointSystem::showClashPoint1(bool flag) {
  m_d->switch0->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;
}

void GXClashPointSystem::showClashPoint2(bool flag) {
  m_d->switch1->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;
}

void GXClashPointSystem::showClashPoint3(bool flag) {
  m_d->switch2->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;
}
void GXClashPointSystem::showClashPoint4(bool flag) {
  m_d->switch3->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;
}
