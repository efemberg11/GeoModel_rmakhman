#include "GDMLInterface/unidentifiedShapeHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoUnidentifiedShape.h"



unidentifiedShapeHandler::unidentifiedShapeHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing unidentifiedShapeHandler!"<<std::endl;
}

void unidentifiedShapeHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //std::cout << "handling for box: name "<< name <<'\n';

  GeoShape* unidentified=new GeoUnidentifiedShape(name);
  theController->saveSolid(name,unidentified);
}
