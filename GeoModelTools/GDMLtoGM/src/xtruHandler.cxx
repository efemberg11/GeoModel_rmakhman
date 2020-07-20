#include "GDMLInterface/xtruHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoBox.h"



xtruHandler::xtruHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void xtruHandler::ElementHandle()
{
  std::cout<<" this is xtruHandler. No implementation yet, returning box"<<std::endl;
  StopLoop(true);
  std::string name=getAttributeAsString("name");

  GeoShape* box=new GeoBox(50.,50.,50.);
  theController->saveSolid(name,box);
}
