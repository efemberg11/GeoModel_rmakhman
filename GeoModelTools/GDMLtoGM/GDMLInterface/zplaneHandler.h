
#ifndef zplaneHandler_H
#define zplaneHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include <string>

class GDMLController;

struct zPlane {
	double rmin;
	double rmax;
	double z;
};

class zplaneHandler:public GDMLHandler {
public:
	zplaneHandler(std::string n, GDMLController* c);
	void ElementHandle();
	zPlane& getZPlane();
private:
	zPlane p;
};


#endif /* end of include guard:  */
