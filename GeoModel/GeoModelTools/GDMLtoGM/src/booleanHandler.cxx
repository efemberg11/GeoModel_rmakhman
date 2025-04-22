#include "GDMLInterface/booleanHandler.h"

booleanHandler::booleanHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
}
void booleanHandler::ElementHandle() 
{
	theShape=0;
	ref=getAttributeAsString("ref");
	theShape=theController->retrieveSolid(ref);
}
GeoShape* booleanHandler::getSolid() 
{
	return theShape;
}
