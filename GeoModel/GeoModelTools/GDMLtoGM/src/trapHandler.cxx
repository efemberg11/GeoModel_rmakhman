#include "GDMLInterface/trapHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoTrap.h"



trapHandler::trapHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void trapHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  double z=lunit*getAttributeAsDouble("z");
  double theta=aunit*getAttributeAsDouble("theta");
  double phi=aunit*getAttributeAsDouble("phi");
  double y1=lunit*getAttributeAsDouble("y1");
  double x1=lunit*getAttributeAsDouble("x1");
  double x2=lunit*getAttributeAsDouble("x2");
  double alpha1=aunit*getAttributeAsDouble("alpha1");
  double y2=lunit*getAttributeAsDouble("y2");
  double x3=lunit*getAttributeAsDouble("x3");
  double x4=lunit*getAttributeAsDouble("x4");
  double alpha2=aunit*getAttributeAsDouble("alpha2");

  GeoShape* trap=new GeoTrap(z,theta,phi,y1,x1,x2,alpha1,y2,x3,x4,alpha2);
  theController->saveSolid(name,trap);
}
