
#ifndef replicate_axisHandler_H
#define replicate_axisHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include <string>

class GDMLHandler;
class GeoPhysVol;
class GeoTransform;

class replicate_axisHandler:public GDMLHandler {
public:

	replicate_axisHandler(std::string n, GDMLController* c);
	void ElementHandle();

private:

};


#endif /* end of include guard:  */
