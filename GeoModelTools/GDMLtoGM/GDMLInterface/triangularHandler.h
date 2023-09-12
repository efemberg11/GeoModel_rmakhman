
#ifndef triangularHandler_H
#define triangularHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include "GeoModelKernel/GeoFacet.h"

#include <string>
#include <iostream>

class GDMLController;

class triangularHandler:public GDMLHandler {
public:
	triangularHandler(std::string n, GDMLController* c);
	void ElementHandle();
};


#endif /* end of include guard:  */
