#include "GDMLInterface/torusHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoTorus.h"



torusHandler::torusHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void torusHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for tube: name" << name << '\n';
  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  double rmin=lunit*getAttributeAsDouble("rmin");
  double rmax=lunit*getAttributeAsDouble("rmax");
  double rtor=lunit*getAttributeAsDouble("rtor");
  double startphi=aunit*getAttributeAsDouble("startphi");
  double deltaphi=aunit*getAttributeAsDouble("deltaphi");

  GeoShape* torus=new GeoTorus(rmin,rmax,rtor,startphi,deltaphi);
  theController->saveSolid(name,torus);
}
