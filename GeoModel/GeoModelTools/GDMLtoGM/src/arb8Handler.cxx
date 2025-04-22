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
  
  double v2x=unit*getAttributeAsDouble("v2x");
  double v2y=unit*getAttributeAsDouble("v2y");
  
  double v3x=unit*getAttributeAsDouble("v3x");
  double v3y=unit*getAttributeAsDouble("v3y");
  
  double v4x=unit*getAttributeAsDouble("v4x");
  double v4y=unit*getAttributeAsDouble("v4y");
  
  double v5x=unit*getAttributeAsDouble("v5x");
  double v5y=unit*getAttributeAsDouble("v5y");
  
  double v6x=unit*getAttributeAsDouble("v6x");
  double v6y=unit*getAttributeAsDouble("v6y");
  
  double v7x=unit*getAttributeAsDouble("v7x");
  double v7y=unit*getAttributeAsDouble("v7y");
  
  double v8x=unit*getAttributeAsDouble("v8x");
  double v8y=unit*getAttributeAsDouble("v8y");
  
  double dz=unit*getAttributeAsDouble("dz");

  std::vector<double> face1x={v1x,v2x,v3x,v4x};
  std::vector<double> face1y={v1y,v2y,v3y,v4y};
  double areaFace1=0;
  for (int i=0;i<3;i++)
  {
    areaFace1+=face1x[i]*face1y[i+1]-face1x[i+1]*face1y[i];
  }
  bool clockwise=areaFace1<0?true:false;

  if (clockwise)
  {
    theVertices.push_back(GeoTwoVector(v1x,v1y));
    theVertices.push_back(GeoTwoVector(v4x,v4y));
    theVertices.push_back(GeoTwoVector(v3x,v3y));
    theVertices.push_back(GeoTwoVector(v2x,v2y));
  }
  else 
  {
    theVertices.push_back(GeoTwoVector(v1x,v1y));
    theVertices.push_back(GeoTwoVector(v2x,v2y));
    theVertices.push_back(GeoTwoVector(v3x,v3y));
    theVertices.push_back(GeoTwoVector(v4x,v4y));
  }
// do the same for face2 (it would be reasonable to assume that face2 follows the same ordering as face1 but one never knows...)
  std::vector<double> face2x={v5x,v6x,v7x,v8x};
  std::vector<double> face2y={v5y,v6y,v7y,v8y};
  double areaFace2=0;
  for (int i=0;i<3;i++)
  {
    areaFace2+=face2x[i]*face2y[i+1]-face2x[i+1]*face2y[i];
  }
  clockwise=areaFace2<0?true:false;

  if (clockwise)
  {
    theVertices.push_back(GeoTwoVector(v5x,v5y));
    theVertices.push_back(GeoTwoVector(v8x,v8y));
    theVertices.push_back(GeoTwoVector(v7x,v7y));
    theVertices.push_back(GeoTwoVector(v6x,v6y));
  }
  else 
  {
    theVertices.push_back(GeoTwoVector(v5x,v5y));
    theVertices.push_back(GeoTwoVector(v6x,v6y));
    theVertices.push_back(GeoTwoVector(v7x,v7y));
    theVertices.push_back(GeoTwoVector(v8x,v8y));
  }

  GeoShape* box=new GeoGenericTrap(dz,theVertices);
  theController->saveSolid(name,box);
}
