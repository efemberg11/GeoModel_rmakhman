#include "GDMLInterface/triangularHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/tessellatedHandler.h"
#include "GeoModelKernel/GeoFacet.h"

triangularHandler::triangularHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
void triangularHandler::ElementHandle() 
{
	std::string pos1=getAttributeAsString("vertex1");
	std::string pos2=getAttributeAsString("vertex2");
	std::string pos3=getAttributeAsString("vertex3");
	position p1=theController->retrievePosition(pos1);
	position p2=theController->retrievePosition(pos2);
	position p3=theController->retrievePosition(pos3);
	GeoFacetVertex v1(p1.x,p1.y,p1.z);
	GeoFacetVertex v2(p2.x,p2.y,p2.z);
	GeoFacetVertex v3(p3.x,p3.y,p3.z);
	GeoFacet* f=new GeoTriangularFacet(v1,v2,v3,GeoFacet::ABSOLUTE);
	XMLHandler* h=theController->XMLStore()->GetHandler(s_currentElement->getParentNode());
	tessellatedHandler* theParentHandler=dynamic_cast<tessellatedHandler*> (h);
	//std::cout<<"adding facet "<<std::endl;
	if (theParentHandler) theParentHandler->addFacet(f);
}
