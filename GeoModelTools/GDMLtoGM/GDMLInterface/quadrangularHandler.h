#ifndef quadrangularHandler_H
#define quadrangularHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include "GeoModelKernel/GeoFacet.h"

#include <string>
#include <iostream>

class GDMLController;

class quadrangularHandler:public GDMLHandler {
public:
	quadrangularHandler(std::string n, GDMLController* c);
	void ElementHandle();
};


#endif /* end of include guard:  */
