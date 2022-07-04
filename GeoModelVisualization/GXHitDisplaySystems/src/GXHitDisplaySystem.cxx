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
  GXHitDisplaySystem *theclass;
  HitDisplaySysController * controller;
  SoSwitch                * switch0;
  SoDrawStyle             * drawStyle;

    static SbColor4f color4f(const QColor& col) {
    return SbColor4f(std::max<float>(0.0f,std::min<float>(1.0f,col.redF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.greenF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.blueF())),
		     1.0);
  }


};

namespace hitdisp {
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
				      tr("Clashpoint files (*.json)"),0,QFileDialog::DontUseNativeDialog);
  SoGroup *switches[]={m_d->switch0};

  if (path!="") {
    m_d->switch0->removeAllChildren();
    std::ifstream i(path.toStdString());
    auto j=json::parse(i);

    try {
      SoCoordinate3 *coords[]={new SoCoordinate3};
      unsigned int   counter[]={0};

      for (const auto& element : j["ClashesReport"]){
	    hitdisp::typeOfClash type=element["typeOfClash"];
	    coords[type]->point.set1Value(counter[type]++,element["x"], element["y"], element["z"]);
      }
      for (int i=0;i<1;i++) {
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

void GXHitDisplaySystem::setPointSize(int size) {
  m_d->drawStyle->pointSize.setValue(size);
}

 
void GXHitDisplaySystem::showHitDisplay1(bool flag) {
  m_d->switch0->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;
}

