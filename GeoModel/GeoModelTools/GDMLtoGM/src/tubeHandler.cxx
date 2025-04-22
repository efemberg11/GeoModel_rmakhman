#include "GDMLInterface/tubeHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoTubs.h"



tubeHandler::tubeHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void tubeHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for tube: name" << name << '\n';

  double rmin=getAttributeAsDouble("rmin");
  double rmax=getAttributeAsDouble("rmax");
  double z=getAttributeAsDouble("z");
  double startphi=getAttributeAsDouble("startphi");
  double deltaphi=getAttributeAsDouble("deltaphi");
  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  GeoShape* tube=new GeoTubs(lunit*rmin,lunit*rmax,lunit*z/2.,aunit*startphi,aunit*deltaphi);
  theController->saveSolid(name,tube);
}
