#include "GDMLInterface/materialrefHandler.h"

materialrefHandler::materialrefHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
}
void materialrefHandler::ElementHandle() 
{
	theMaterial=0;
	ref=getAttributeAsString("ref");
	theMaterial=theController->retrieveMaterial(ref);
}
GeoMaterial* materialrefHandler::getMaterial() 
{
	return theMaterial;
}
