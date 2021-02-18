
#ifndef positionHandler_H
#define positionHandler_H

// ancillary handler to element creation

#include "GDMLInterface/GDMLHandler.h"

#include "GDMLInterface/position.h"

#include <string>

class GDMLController;

class positionHandler:public GDMLHandler {
public:
<<<<<<< HEAD
	positionHandler(std::string n, GDMLController* c);
	void ElementHandle();
	position& getPosition();
=======
	positionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		std::string hName=GetName();
		if (hName=="position" || hName=="firstposition")
		{
			//std::cout<<"handling position "<<std::endl;
			p.name="";
			p.x=p.y=p.z=0;
                        bool isPresent;
                        double unit=1.0;
			p.name=getAttributeAsString("name", isPresent);
			unit=getAttributeAsDouble("unit", isPresent);
			p.x=unit*getAttributeAsDouble("x", isPresent);
			p.y=unit*getAttributeAsDouble("y", isPresent);
			p.z=unit*getAttributeAsDouble("z", isPresent);
			p.v3d=GeoTrf::Vector3D(p.x,p.y,p.z);
		    
			theController->savePosition(p.name,p);
                        //std::cout<<"handling position, done "<<std::endl;
		}
		else if (hName=="positionref"|| hName=="firstpositionref")
		{
			std::string ref=getAttributeAsString("ref");
			p=theController->retrievePosition(ref);
		}
	}
	position& getPosition() {return p;}
>>>>>>> gdml2gm: handle properly optional parameted in physvol, position and rotation handlers
private:
	position p;
};


#endif /* end of include guard:  */
