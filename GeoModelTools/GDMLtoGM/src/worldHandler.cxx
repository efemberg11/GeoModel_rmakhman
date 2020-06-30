#include "GDMLInterface/worldHandler.h"

worldHandler::worldHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void worldHandler::ElementHandle() 
{
	ref=getAttributeAsString("ref");
	//GeoVolume  w=theController->retrieveLogicalVolume(stripPointer(ref));
	GeoVolume  w=theController->retrieveLogicalVolume(ref);
	theController->setWorld(w);
}
	
