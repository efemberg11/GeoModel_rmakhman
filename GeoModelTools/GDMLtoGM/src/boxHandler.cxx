#include "GDMLInterface/boxHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoBox.h"



boxHandler::boxHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void boxHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //std::cout << "handling for box: name "<< name <<'\n';

  double x=getAttributeAsDouble("x");
  double y=getAttributeAsDouble("y");
  double z=getAttributeAsDouble("z");
  double lunit=getAttributeAsDouble("lunit");
  GeoShape* box=new GeoBox(lunit*x/2.,lunit*y/2.,lunit*z/2.);
  theController->saveSolid(name,box);
}
