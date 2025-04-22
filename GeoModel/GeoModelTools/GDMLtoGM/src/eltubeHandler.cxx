#include "GDMLInterface/eltubeHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoEllipticalTube.h"



eltubeHandler::eltubeHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void eltubeHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for eltube: name" << name << '\n';

  double dx=getAttributeAsDouble("dx");
  double dy=getAttributeAsDouble("dy");
  double dz=getAttributeAsDouble("dz");
  double lunit=getAttributeAsDouble("lunit");
  GeoShape* eltube=new GeoEllipticalTube(lunit*dx,lunit*dy,lunit*dz);
  theController->saveSolid(name,eltube);
}
