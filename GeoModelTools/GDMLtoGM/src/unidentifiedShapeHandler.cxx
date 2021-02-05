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
  
  std::cout<<"\n***********************************************************************************\n\n"<<std::endl;
  std::cout<<"Warning: this is unidentifiedShapeHandler called for "<<m_name<<" name "<<name<<"!!!"<<std::endl;
  std::cout<<m_name<<" is not a supported shape yet!!"<<std::endl;
  std::cout<<"\n***********************************************************************************\n\n"<<std::endl;

  GeoShape* unidentified=new GeoUnidentifiedShape(name);
  theController->saveSolid(name,unidentified);
}
