#include "GDMLInterface/arb8Handler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoBox.h"



arb8Handler::arb8Handler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void arb8Handler::ElementHandle()
{
  std::cout<<" this is arb8Handler. No implementation yet, returning box"<<std::endl;
  std::string name=getAttributeAsString("name");

  GeoShape* box=new GeoBox(50.,50.,50.);
  theController->saveSolid(name,box);
}
