#include "GDMLInterface/coneHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoCons.h"



coneHandler::coneHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void coneHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for cone: name" << name << '\n';

  double rmin1=getAttributeAsDouble("rmin1");
  double rmax1=getAttributeAsDouble("rmax1");
  double rmin2=getAttributeAsDouble("rmin2");
  double rmax2=getAttributeAsDouble("rmax2");
  double z=getAttributeAsDouble("z");
  double startphi=getAttributeAsDouble("startphi");
  double deltaphi=getAttributeAsDouble("deltaphi");
  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  GeoShape* cone=new GeoCons(lunit*rmin1,lunit*rmin2,lunit*rmax1,lunit*rmax2,lunit*z/2.,aunit*startphi,aunit*deltaphi);
  theController->saveSolid(name,cone);
}
