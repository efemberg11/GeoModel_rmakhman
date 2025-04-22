#include "GDMLInterface/xtruHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>
#include <algorithm>

#include "GeoModelXMLParser/XercesParser.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Units.h"
#include "GDMLInterface/sectionHandler.h"
#include "GDMLInterface/twoDimVertexHandler.h"



xtruHandler::xtruHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void xtruHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");
  double lunit=getAttributeAsDouble("lunit",GeoModelKernelUnits::mm); 
  //std::cout <<" handling for xtru "<<name<<std::endl;  
  // This interface is only for GeoSimplePolygonBrep.Checks are performed to
  // ensure a GSPB can be built out of the parameters being collected, in any
  // other case an exception is thrown

  GeoSimplePolygonBrep* pgsb;
  
  std::vector<section> theSections;
  std::vector<twoDVtx> theVertices;
  
  StopLoop(true);
  xercesc::DOMNode *child;
  
  for (child=GeoModelTools::XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
  {
	if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) 
	{
		GeoModelTools::XercesParser::elementLoop(child);
		XMLHandler *h=theController->XMLStore()->GetHandler(child);
		sectionHandler* sH=dynamic_cast<sectionHandler*>(h);
		if (sH)
		{
			section s=sH->getSection();
			theSections.push_back(s);
		}
		twoDimVertexHandler* tH=dynamic_cast<twoDimVertexHandler*>(h);
		if (tH)
		{
			twoDVtx d=tH->getVtx();
			theVertices.push_back(d);
		}
	}
  }
  
  if (theSections.size() != 2) throw; // we want a maximum of two sections
  if (theSections[0].scalingFactor != 1. || theSections[1].scalingFactor != 1.) throw; // no scaling
  if ((theSections[0].xOffset!=0 || theSections[0].yOffset!=0) || (theSections[1].xOffset!=0 || theSections[1].yOffset!=0)) throw; // no offset
  if ((theSections[0].zPosition+theSections[1].zPosition) != 0) throw;  // centered at z=0
  
  // it looks like the xtru is well defined. Let's calculate the area of the 
  // surfaces (to check that vertices are provided anti-clockwise)
  
  double area=0;
  int nPoints=theVertices.size();
  int iMinus=nPoints-1;
  for (int i=0;i<nPoints;i++)
  {
	area += (theVertices[iMinus].xv+theVertices[i].xv)*(theVertices[iMinus].yv-theVertices[i].yv);
	iMinus=i;
  }
  
  bool clockwise=area<0?false:true;
  if (clockwise)  // theVertices has to be reversed
    std::reverse(theVertices.begin(),theVertices.end());
  
  pgsb=new GeoSimplePolygonBrep(std::fabs(lunit*theSections[1].zPosition));
  for (auto vt: theVertices) 
  {
  	pgsb->addVertex(lunit*vt.xv,lunit*vt.yv);
  }
  
  
  theController->saveSolid(name,pgsb);
}
