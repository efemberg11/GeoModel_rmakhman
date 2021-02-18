
#ifndef rotationHandler_H
#define rotationHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/rotation.h"

class GDMLController;

class rotationHandler:public GDMLHandler {
public:
<<<<<<< HEAD
	rotationHandler(std::string n, GDMLController* c);
	void ElementHandle();
	rotation& getRotation();
=======
	rotationHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		std::string hName=GetName();
		//std::cout<<" handler for rotation "<<std::endl;
		if (hName=="rotation"|| hName=="firstrotation")
		{
			p.name="";
			p.rotx=p.roty=p.rotz=0;
                        double unit = 1.0;
                        bool isPresent;
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
	rotation& getRotation() {return p;}
private:
>>>>>>> gdml2gm: handle properly optional parameted in physvol, position and rotation handlers
	rotation p;
};


#endif /* end of include guard:  */
