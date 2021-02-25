#include "GDMLInterface/rotationHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLController.h"

rotationHandler::rotationHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void rotationHandler::ElementHandle() 
{
	std::string hName=GetName();
	//std::cout<<" handling for rotation "<<std::endl;
	if (hName=="rotation"|| hName=="firstrotation")
	{
		p.name="";
		p.rotx=p.roty=p.rotz=0;
		p.name=getAttributeAsString("name");
		double unit=getAttributeAsDouble("unit");
		//std::cout<< " rotation: unit "<<unit<<std::endl;
		p.rotx=unit*getAttributeAsDouble("x");
		p.roty=unit*getAttributeAsDouble("y");
		p.rotz=unit*getAttributeAsDouble("z");
			
		theController->saveRotation(p.name,p);
	}
	else if (hName=="rotationref"||hName=="firstrotationref")
	{
		std::string ref=getAttributeAsString("ref");
		p=theController->retrieveRotation(ref);
	}
}
rotation& rotationHandler::getRotation() 
{
	return p;
}
