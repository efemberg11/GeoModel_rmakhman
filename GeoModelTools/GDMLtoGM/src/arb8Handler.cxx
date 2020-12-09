#include "GDMLInterface/arb8Handler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoGenericTrap.h"



arb8Handler::arb8Handler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void arb8Handler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  
  GeoGenericTrapVertices theVertices;
  
  double unit=getAttributeAsDouble("lunit");
  
  double v1x=unit*getAttributeAsDouble("v1x");
  double v1y=unit*getAttributeAsDouble("v1y");
  GeoTwoVector v1(v1x,v1y);
  theVertices.push_back(v1);
  
  double v2x=unit*getAttributeAsDouble("v2x");
  double v2y=unit*getAttributeAsDouble("v2y");
  GeoTwoVector v2(v2x,v2y);
  theVertices.push_back(v2);
  
  double v3x=unit*getAttributeAsDouble("v3x");
  double v3y=unit*getAttributeAsDouble("v3y");
  GeoTwoVector v3(v3x,v3y);
  theVertices.push_back(v3); 
  
  double v4x=unit*getAttributeAsDouble("v4x");
  double v4y=unit*getAttributeAsDouble("v4y");
  GeoTwoVector v4(v4x,v4y);
  theVertices.push_back(v4); 
  
  double v5x=unit*getAttributeAsDouble("v5x");
  double v5y=unit*getAttributeAsDouble("v5y");
  GeoTwoVector v5(v5x,v5y);
  theVertices.push_back(v5);
  
  double v6x=unit*getAttributeAsDouble("v6x");
  double v6y=unit*getAttributeAsDouble("v6y");
  GeoTwoVector v6(v6x,v6y);
  theVertices.push_back(v6);
  
  double v7x=unit*getAttributeAsDouble("v7x");
  double v7y=unit*getAttributeAsDouble("v7y");
  GeoTwoVector v7(v7x,v7y);
  theVertices.push_back(v7); 
  
  double v8x=unit*getAttributeAsDouble("v8x");
  double v8y=unit*getAttributeAsDouble("v8y");
  GeoTwoVector v8(v8x,v8y);
  theVertices.push_back(v8); 
  
  double dz=unit*getAttributeAsDouble("dz");

  GeoShape* box=new GeoGenericTrap(dz,theVertices);
  theController->saveSolid(name,box);
}
