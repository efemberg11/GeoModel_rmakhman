
#ifndef tessellatedHandler_H
#define tessellatedHandler_H

class GeoFacet;

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class tessellatedHandler:public GDMLHandler {
public:
	tessellatedHandler(std::string, GDMLController*);
	void ElementHandle();
	void postLoopHandling();
	void addFacet(GeoFacet*);
	
	std::vector<GeoFacet*> facets;
	std::string name;
};


#endif /* end of include guard:  */
