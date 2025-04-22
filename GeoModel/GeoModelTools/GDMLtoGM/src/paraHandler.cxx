#include "GDMLInterface/paraHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoPara.h"



paraHandler::paraHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void paraHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for tube: name" << name << '\n';

  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  double dx=lunit*getAttributeAsDouble("x");
  double dy=lunit*getAttributeAsDouble("y");
  double dz=lunit*getAttributeAsDouble("z");
  double alpha=aunit*getAttributeAsDouble("alpha");
  double theta=aunit*getAttributeAsDouble("theta");
  double phi=aunit*getAttributeAsDouble("phi");
  
  GeoShape* tube=new GeoPara(dx,dy,dz,alpha,theta,phi);
  theController->saveSolid(name,tube);
}
