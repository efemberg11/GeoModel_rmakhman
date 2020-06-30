
#ifndef atomHandler_H
#define atomHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/isotopeHandler.h"
#include <string>
#include <iostream>

class atomHandler:public GDMLHandler {
public:
	atomHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		unit=value=atomic_weight=0;
		unit=getAttributeAsDouble("unit");
		value=getAttributeAsDouble("value");
		atomic_weight=unit*value;
	}
	void postLoopHandling()
	{
		XMLHandler* h=theController->XMLStore()->GetHandler(s_currentElement->getParentNode());
		// if (h) std::cout<<h->GetName();
		isotopeHandler* theParentHandler=dynamic_cast<isotopeHandler*> (h);
		if (theParentHandler) theParentHandler->setAtomicWeight(atomic_weight);
		//else std::cout<<" Help!!!! wrong type of Handler!!!!"<<std::endl;
	}
	double atomicWeight() {return atomic_weight;}
private:
	double unit;
	double value;
	double atomic_weight;
};


#endif /* end of include guard:  */
