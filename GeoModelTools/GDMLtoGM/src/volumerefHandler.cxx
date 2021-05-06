#include "GDMLInterface/volumerefHandler.h"

volumerefHandler::volumerefHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}

void volumerefHandler::ElementHandle() 
{
	ref=getAttributeAsString("ref");
	std::cout<<" volumeref name "<<ref<<std::endl;
	theVolume=theController->retrieveLogicalVolume(ref);
	//theVolume=theController->retrieveLogicalVolume(stripPointer(ref));
}
	
GeoVolume volumerefHandler::getVolume() 
{
	return theVolume;
}
