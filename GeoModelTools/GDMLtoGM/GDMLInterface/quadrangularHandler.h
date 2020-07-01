
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
		GeoFacet* f=new GeoQuadrangularFacet(theController->retrievePosition(pos1).v3d,
		                                     theController->retrievePosition(pos2).v3d,
						     theController->retrievePosition(pos3).v3d,
						     theController->retrievePosition(pos4).v3d, 
						     GeoFacet::GeoFacetVertexType::ABSOLUTE);
		
		XMLHandler* h=theController->XMLStore()->GetHandler(s_currentElement->getParentNode());
                tessellatedHandler* theParentHandler=dynamic_cast<tessellatedHandler*> (h);
                if (theParentHandler) theParentHandler->addFacet(f);
	}
};


#endif /* end of include guard:  */
