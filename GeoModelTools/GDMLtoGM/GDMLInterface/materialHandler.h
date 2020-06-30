
#ifndef materialHandler_H
#define materialHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/fraction.h"

#include "GeoModelKernel/GeoMaterial.h"

#include "GeoModelXMLParser/XercesParser.h"

class materialHandler:public GDMLHandler {
public:

	materialHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		//name=stripPointer(getAttributeAsString("name"));
		name=getAttributeAsString("name");
		density=0;

		StopLoop(true);
		xercesc::DOMNode *child;

		for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
		{
			if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
				XMLHandler *h=theController->XMLStore()->GetHandler(child);
				std::string nH=h->GetName();
				//std::cout<<" handler name "<<nH<<std::endl;
				if (nH=="fraction") {
					fractionHandler* fH=dynamic_cast<fractionHandler*>(h);
					if (!fH) std::cout<<" something is wrong! can not retrieve fractionHandler!!!"<<std::endl;
					XercesParser::elementLoop(child);
					addFraction(fH->getFraction());
				}
				// else if (nH=="D") {std::cout<<" handling for density"<<std::endl;XercesParser::elementLoop(child);}
			}
		}
	}
	void postLoopHandling()
	{
		//std::cout<<"new material "<<name<<" density "<<density<<std::endl;
		GeoMaterial* newMaterial=new GeoMaterial(stripPointer(name),density);
		for (auto f: fractionList)
		{
			//std::cout<<"\t fraction "<<" f.ref "<<f.ref<<" f.weight "<<f.weight<<std::endl;
			GeoElement* e=theController->retrieveElement(stripPointer(f.ref));
			newMaterial->add(e,f.weight);
		}
		newMaterial->lock();
		//std::cout<<" done defining material "<<name<<std::endl;
		theController->saveMaterial(name,newMaterial);
	}
	void addFraction(fraction f) {fractionList.push_back(f);}
	void setDensity(double d) {density=d;}
private:
	std::string name;
	double density;
  std::vector<fraction> fractionList;
};


#endif /* end of include guard:  */
