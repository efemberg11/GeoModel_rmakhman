#include "GDMLInterface/polyhedraHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelXMLParser/XercesParser.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GDMLInterface/zplaneHandler.h"



polyhedraHandler::polyhedraHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void polyhedraHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for polycone: name" << name << '\n';

  unsigned int nSides=getAttributeAsInt("numsides");
  double startphi=getAttributeAsDouble("startphi");
  double deltaphi=getAttributeAsDouble("deltaphi");
  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  GeoPgon* pgon=new GeoPgon(aunit*startphi,aunit*deltaphi,nSides);
  
  StopLoop(true);
  xercesc::DOMNode *child;
  
  for (child=GeoModelTools::XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
  {
	if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) 
	{
		GeoModelTools::XercesParser::elementLoop(child);
		XMLHandler *h=theController->XMLStore()->GetHandler(child);
        if(h){
            zplaneHandler* zplH=dynamic_cast<zplaneHandler*>(h);
            if (zplH)
            {
                zPlane p=zplH->getZPlane();
                pgon->addPlane(lunit*p.z,lunit*p.rmin,lunit*p.rmax);
            }
            
        }
        else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
	}
  }
  
  theController->saveSolid(name,pgon);
}
