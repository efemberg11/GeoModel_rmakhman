#include "GDMLInterface/tessellatedHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelKernel/GeoTessellatedSolid.h"
#include "GeoModelKernel/GeoFacet.h"



tessellatedHandler::tessellatedHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing boxHandler!"<<std::endl;
}

void tessellatedHandler::ElementHandle()
{
  name=getAttributeAsString("name");
}

void tessellatedHandler::addFacet(GeoFacet* f)
{
	static int nEntry=0;
	//std::cout<<" adding facet "<<nEntry<<std::endl;
	facets.push_back(f);
	nEntry++;
}

void tessellatedHandler::postLoopHandling() 
{
  GeoTessellatedSolid* tessellated=new GeoTessellatedSolid();
  //std::cout<<" tessellated solid "<<name<<" nr. of facets: "<<facets.size()<<std::endl;
  for (auto facet: facets)
  	tessellated->addFacet(facet);
  theController->saveSolid(name,tessellated);
}
