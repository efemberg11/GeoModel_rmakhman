#include "GDMLInterface/polyconeHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

#include "GeoModelXMLParser/XercesParser.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GDMLInterface/zplaneHandler.h"



polyconeHandler::polyconeHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing tubeHandler!"<<std::endl;
}

void polyconeHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //  std::cout << "handling for polycone: name" << name << '\n';

  double startphi=getAttributeAsDouble("startphi");
  double deltaphi=getAttributeAsDouble("deltaphi");
  double lunit=getAttributeAsDouble("lunit");
  double aunit=getAttributeAsDouble("aunit");
  GeoPcon* pcone=new GeoPcon(aunit*startphi,aunit*deltaphi);
  
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
                pcone->addPlane(lunit*p.z,lunit*p.rmin,lunit*p.rmax);
            }
        }
        else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
	}
  }
  
  theController->saveSolid(name,pcone);
}
