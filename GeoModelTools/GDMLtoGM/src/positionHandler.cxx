#include "GDMLInterface/positionHandler.h"

#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/position.h"

#include "GeoModelKernel/GeoDefinitions.h"

positionHandler::positionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void positionHandler::ElementHandle() 
{
	std::string hName=GetName();
	if (hName=="position" || hName=="firstposition")
	{
		//std::cout<<"handling position "<<std::endl;
        //NB: unit and all the other fields are not mandatory in gdml
        bool isPresent;
        double unit=1.0;
		p.name="";
		p.x=p.y=p.z=0;
		p.name=getAttributeAsString("name", isPresent);
		double unit=getAttributeAsDouble("unit", isPresent);
		p.x=unit*getAttributeAsDouble("x", isPresent);
		p.y=unit*getAttributeAsDouble("y", isPresent);
		p.z=unit*getAttributeAsDouble("z", isPresent);
			
		p.v3d=GeoTrf::Vector3D(p.x,p.y,p.z);
		
		theController->savePosition(p.name,p);
	}
	else if (hName=="positionref"|| hName=="firstpositionref")
	{
		std::string ref=getAttributeAsString("ref");
		p=theController->retrievePosition(ref);
	}
}
position& positionHandler::getPosition() 
{
	return p;
}
