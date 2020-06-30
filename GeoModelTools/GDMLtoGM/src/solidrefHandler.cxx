#include "GDMLInterface/solidrefHandler.h"

solidrefHandler::solidrefHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
}
void solidrefHandler::ElementHandle() 
{
	theShape=0;
	ref=getAttributeAsString("ref");
	theShape=theController->retrieveSolid(ref);
}
GeoShape* solidrefHandler::getSolid() 
{
	return theShape;
}
