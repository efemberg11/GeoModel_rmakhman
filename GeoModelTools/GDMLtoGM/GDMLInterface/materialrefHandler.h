
#ifndef materialrefHandler_H
#define materialrefHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoMaterial.h"

class materialrefHandler:public GDMLHandler {
public:

	materialrefHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		theMaterial=0;
		ref=getAttributeAsString("ref");
		theMaterial=theController->retrieveMaterial(ref);
	}
	GeoMaterial* getMaterial() {return theMaterial;}
private:
	std::string ref="";
	GeoMaterial* theMaterial=0;
};


#endif /* end of include guard:  */
