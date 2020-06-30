
#ifndef densityHandler_H
#define densityHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/materialHandler.h"
#include <string>
#include <iostream>

class densityHandler:public GDMLHandler {
public:
	densityHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		unit=value=density=0;
		unit=getAttributeAsDouble("unit");
		value=getAttributeAsDouble("value");
		density=unit*value;
	}
	void postLoopHandling()
	{
		XMLHandler* h=theController->XMLStore()->GetHandler(s_currentElement->getParentNode());
		// if (h) std::cout<<h->GetName();
		materialHandler* theParentHandler=dynamic_cast<materialHandler*> (h);
		if (theParentHandler) theParentHandler->setDensity(density);
		else std::cout<<" densityHandler: Help!!!! wrong type of Handler!!!!"<<std::endl;
	}
private:
	double unit;
	double value;
	double density;
};


#endif /* end of include guard:  */
