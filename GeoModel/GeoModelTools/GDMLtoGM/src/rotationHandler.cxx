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
        double unit = 1.0;
        bool isPresent;
        p.name="";
		p.rotx=p.roty=p.rotz=0;
		p.name=getAttributeAsString("name", isPresent);
		unit=getAttributeAsDouble("unit", isPresent);
		//std::cout<< " rotation: unit "<<unit<<std::endl;
		p.rotx=unit*getAttributeAsDouble("x", isPresent);
		p.roty=unit*getAttributeAsDouble("y", isPresent);
		p.rotz=unit*getAttributeAsDouble("z", isPresent);
			
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
