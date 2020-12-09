#include "GDMLInterface/elementHandler.h"

#include "GDMLInterface/isotope.h"

#include "GeoModelKernel/GeoElement.h"
#include "GDMLInterface/element.h"
#include "GDMLInterface/fractionHandler.h"
#include "GDMLInterface/atomHandler.h"

#include "GeoModelXMLParser/XMLHandler.h"
#include "GeoModelXMLParser/XercesParser.h"

elementHandler::elementHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}

void elementHandler::ElementHandle() {
	e.clear();
	e.name=stripPointer(getAttributeAsString("name"));
	//std::cout<<" element "<<e.name<<" being defined"<<std::endl;

	isAtom=false;

	symbol=getAttributeAsString("formula","");
	Z=getAttributeAsInt("Z",0);

	StopLoop(true);
	xercesc::DOMNode *child;

	for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
			XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
            if(h){
                std::string nH=h->GetName();
                //std::cout<<" handler name "<<nH<<std::endl;
                if (nH=="atom") {
                    atomHandler* aH=dynamic_cast<atomHandler*>(h);
                    if (!aH) std::cout<<" something is wrong! can not retrieve atomHandler!!!"<<std::endl;
                    else {
                        isAtom=true;
                        weight=aH->atomicWeight();
                    }
                    break;
                }
                else if (nH=="fraction") {
                    fractionHandler* fH=dynamic_cast<fractionHandler*>(h);
                    if (!fH) std::cout<<" something is wrong! can not retrieve fractionHandler!!!"<<std::endl;
                    e.addIsotope(fH->getFraction());
                }
                else std::cout << "elementHandler: don't know what this might be!!!"<<nH<<std::endl;
                
            }
            else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
			
		}
	}
}

void elementHandler::postLoopHandling()
{
	GeoElement* newElement=0;
	if (isAtom)
	{
		if (symbol.empty()) std::cout<<"elementHandle warning: symbol not defined"<<std::endl;
		if (!Z) std::cout<<"elementHandle warning: Z is 0"<<std::endl;
		newElement=new GeoElement(e.name,symbol,Z,weight);
	}
	else
	{
		fraction f=e.isotopeList[0];
		//std::cout<<"new element "<<e.name<<" fraction "<<f.weight<<" name "<<f.ref<<std::endl;
		isotope i=theController->retrieveIsotope(f.ref);
		//std::cout <<"\t isotope name "<<i.name<<" symbol "<<i.symbol<<" N "<<i.N<<" Z "<<i.Z<<" weight "<<i.atomic_weight<<std::endl;
		newElement=new GeoElement(e.name,i.symbol,i.Z,i.atomic_weight);
	}
	theController->saveElement(e.name,newElement);
}
