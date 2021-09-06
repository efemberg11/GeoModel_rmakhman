#include "GDMLInterface/materialHandler.h"

#include "GDMLInterface/fractionHandler.h"
#include "GDMLInterface/densityHandler.h"
#include "GDMLInterface/atomHandler.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"

#include "GeoModelXMLParser/XercesParser.h"

materialHandler::materialHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void materialHandler::ElementHandle() 
{
	name=getAttributeAsString("name");
	m_Z=getAttributeAsInt("Z",explicitMaterial);
	std::cout<<" new material "<<name;
	if (explicitMaterial) std::cout<<" Z= "<<m_Z<<std::endl;
	else std::cout<<std::endl;
	density=0;
	StopLoop(true);
// clear fractionList for new material (D.Casper)
	fractionList.clear();

	xercesc::DOMNode *child;

	for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) 
		{
			XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
                	if(h)
			{
                    		std::string nH=h->GetName();
                    		std::cout<<" handler name "<<nH<<std::endl;
                    		if (nH=="fraction") 
				{
                        		fractionHandler* fH=dynamic_cast<fractionHandler*>(h);
                        		if (!fH) std::cout<<" something is wrong! can not retrieve fractionHandler!!!"<<std::endl;
                        		addFraction(fH->getFraction());
                    		}
                    		else if (nH=="D") 
				{
                        		densityHandler* dH=dynamic_cast<densityHandler*>(h);
                        		if (!dH) std::cout<<" something is wrong! can not retrieve densityHandler!!!"<<std::endl;
                        		density=dH->getDensity();
                    		}
                    		else if (nH=="atom") 
				{
					atomHandler* aH=dynamic_cast<atomHandler*>(h);
					if (!aH) std::cout<<" something is wrong! can not retrieve atomHandler!!!"<<std::endl;
					m_A=aH->atomicWeight();
                    		}
                	}
                	else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
            	}
	}
}
void materialHandler::postLoopHandling()
{
	std::cout<<"new material "<<name<<" density "<<density<<std::endl;
	GeoMaterial* newMaterial=new GeoMaterial(stripPointer(name),density);
	if (explicitMaterial)
	{
		std::cout<<" creating an explicit material: name "<<name<<" Z="<<m_Z<<" A="<<m_A<<std::endl;
		GeoElement* newElement=new GeoElement(name,name,m_Z,m_A);
		theController->saveElement(name,newElement);
		newMaterial->add(newElement,1.0);
	}
	else for (auto f: fractionList)
	{
		std::cout<<"\t fraction "<<" f.ref "<<f.ref<<" f.weight "<<f.weight<<std::endl;
		GeoElement* e=theController->retrieveElement(stripPointer(f.ref));
		newMaterial->add(e,f.weight);
	}
	newMaterial->lock();
	//std::cout<<" done defining material "<<name<<std::endl;
	theController->saveMaterial(name,newMaterial);
}
void materialHandler::addFraction(fraction f) 
{
	fractionList.push_back(f);
}
void materialHandler::setDensity(double d) 
{
	density=d;
}
