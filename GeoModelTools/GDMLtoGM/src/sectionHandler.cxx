#include "GDMLInterface/sectionHandler.h"

#include "GDMLInterface/GDMLController.h"

#include "GeoModelXMLParser/XercesParser.h"

sectionHandler::sectionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void sectionHandler::ElementHandle() 
{
	s.scalingFactor=getAttributeAsDouble("scalingFactor");
	s.xOffset=getAttributeAsDouble("xOffset");
	s.yOffset=getAttributeAsDouble("yOffset");
	s.zOrder=getAttributeAsInt("zOrder");
	s.zPosition=getAttributeAsDouble("zPosition");
}
section& sectionHandler::getSection() 
{
	return s;
}
