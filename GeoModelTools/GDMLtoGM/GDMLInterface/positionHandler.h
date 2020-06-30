
#ifndef positionHandler_H
#define positionHandler_H

// ancillary handler to element creation

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/position.h"
class positionHandler:public GDMLHandler {
public:
	positionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		std::string hName=GetName();
		if (hName=="position" || hName=="firstposition")
		{
			//std::cout<<"handling position "<<std::endl;
			p.name="";
			p.x=p.y=p.z=0;
			p.name=getAttributeAsString("name");
			double unit=getAttributeAsDouble("unit");
			p.x=unit*getAttributeAsDouble("x");
			p.y=unit*getAttributeAsDouble("y");
			p.z=unit*getAttributeAsDouble("z");
			
			p.v3d=GeoTrf::Vector3D(p.x,p.y,p.z);
			//  p.v3d=GeoTrf::Translation3D(v3d);
		
			theController->savePosition(p.name,p);
		}
		else if (hName=="positionref"|| hName=="firstpositionref")
		{
			std::string ref=getAttributeAsString("ref");
			p=theController->retrievePosition(ref);
		}
	}
	position& getPosition() {return p;}
private:
	position p;
};


#endif /* end of include guard:  */
