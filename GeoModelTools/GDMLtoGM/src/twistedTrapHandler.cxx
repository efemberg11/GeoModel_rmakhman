#include "GDMLInterface/twistedTrapHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoTwistedTrap.h"



twistedTrapHandler::twistedTrapHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing twistedTrapHandler!"<<std::endl;
}

void twistedTrapHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");
  
  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  
  double alpha=aunit*getAttributeAsDouble("Alph");
  double theta=aunit*getAttributeAsDouble("Theta");
  double phi=aunit*getAttributeAsDouble("Phi");
  double phiTwist=aunit*getAttributeAsDouble("PhiTwist");
  
  double x1=lunit*getAttributeAsDouble("x1");
  double x2=lunit*getAttributeAsDouble("x2");
  double x3=lunit*getAttributeAsDouble("x3");
  double x4=lunit*getAttributeAsDouble("x4");
  double y1=lunit*getAttributeAsDouble("y1");
  double y2=lunit*getAttributeAsDouble("y2");
  double z=lunit*getAttributeAsDouble("z");

  GeoShape* twistedtrap=new GeoTwistedTrap(phiTwist, z, theta, phi, y1, x1, x2, y2, x3, x4, alpha);
  theController->saveSolid(name,twistedtrap);
    
}
