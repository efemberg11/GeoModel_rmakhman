
#ifndef zplaneHandler_H
#define zplaneHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

struct zPlane {
	double rmin;
	double rmax;
	double z;
};

class zplaneHandler:public GDMLHandler {
public:
	zplaneHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		//std::cout<<"handling zplane "<<std::endl;
		p.rmin=getAttributeAsDouble("rmin");
		p.rmax=getAttributeAsDouble("rmax");
		p.z=getAttributeAsDouble("z");
	}
	zPlane& getZPlane() {return p;}
private:
	zPlane p;
};


#endif /* end of include guard:  */
