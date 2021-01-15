#include "GDMLInterface/trdHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoTrd.h"



trdHandler::trdHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void trdHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //std::cout << "handling for box: name "<< name <<'\n';
  double lunit=getAttributeAsDouble("lunit"); 
  double x1=getAttributeAsDouble("x1");
  double x2=getAttributeAsDouble("x2");
  double y1=getAttributeAsDouble("y1");
  double y2=getAttributeAsDouble("y2");
  double z=getAttributeAsDouble("z");

  GeoShape* trd=new GeoTrd(lunit*x1,lunit*x2,lunit*y1,lunit*y2,lunit*z);
  theController->saveSolid(name,trd);
}
