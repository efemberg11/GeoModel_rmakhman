
#ifndef quadrangularHandler_H
#define quadrangularHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GeoModelKernel/GeoFacet.h"
#include <string>
#include <iostream>

class quadrangularHandler:public GDMLHandler {
public:
	quadrangularHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		std::string pos1=getAttributeAsString("vertex1");
		std::string pos2=getAttributeAsString("vertex2");
		std::string pos3=getAttributeAsString("vertex3");
		std::string pos4=getAttributeAsString("vertex4");
		
		position p1=theController->retrievePosition(pos1);
		position p2=theController->retrievePosition(pos2);
		position p3=theController->retrievePosition(pos3);
		position p4=theController->retrievePosition(pos4);
		GeoFacetVertex v1(p1.x,p1.y,p1.z);
		GeoFacetVertex v2(p2.x,p2.y,p2.z);
		GeoFacetVertex v3(p3.x,p3.y,p3.z);
		GeoFacetVertex v4(p4.x,p4.y,p4.z);
		GeoFacet* f=new GeoQuadrangularFacet(v1,v2,v3,v4,GeoFacet::ABSOLUTE);
		
		XMLHandler* h=theController->XMLStore()->GetHandler(s_currentElement->getParentNode());
                tessellatedHandler* theParentHandler=dynamic_cast<tessellatedHandler*> (h);
                if (theParentHandler) theParentHandler->addFacet(f);
	}
};


#endif /* end of include guard:  */
