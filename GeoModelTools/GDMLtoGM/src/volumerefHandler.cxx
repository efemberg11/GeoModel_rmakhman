#include "GDMLInterface/volumerefHandler.h"

volumerefHandler::volumerefHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}

void volumerefHandler::ElementHandle() 
{
	theVolume=std::make_pair(nullptr,nullptr);
	ref=getAttributeAsString("ref");
	theVolume=theController->retrieveLogicalVolume(ref);
	//theVolume=theController->retrieveLogicalVolume(stripPointer(ref));
}
	
GeoVolume volumerefHandler::getVolume() 
{
	return theVolume;
}
