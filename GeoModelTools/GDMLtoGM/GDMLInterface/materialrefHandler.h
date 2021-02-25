
#ifndef materialrefHandler_H
#define materialrefHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoMaterial.h"

#include <string>

class materialrefHandler:public GDMLHandler {
public:

	materialrefHandler(std::string n, GDMLController* c);
	void ElementHandle();
	GeoMaterial* getMaterial();
private:
	std::string ref="";
	GeoMaterial* theMaterial=0;
};


#endif /* end of include guard:  */
