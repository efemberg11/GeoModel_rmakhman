
#ifndef rotationHandler_H
#define rotationHandler_H

#include "GeoModelKernel/GeoDefinitions.h"

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/rotation.h"
class rotationHandler:public GDMLHandler {
public:
	rotationHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
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
	rotation& getRotation() {return p;}
private:
	rotation p;
};


#endif /* end of include guard:  */
