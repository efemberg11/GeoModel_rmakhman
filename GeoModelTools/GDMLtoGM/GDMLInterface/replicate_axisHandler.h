
#ifndef replicate_axisHandler_H
#define replicate_axisHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/Axis.h"

#include <string>

class GDMLHandler;
class GeoPhysVol;
class GeoTransform;

struct axis_params {double w;double o; Axis a;};

class replicate_axisHandler:public GDMLHandler {
public:

	replicate_axisHandler(std::string n, GDMLController* c);
	void ElementHandle();
	axis_params getAxisParameters() {return parameters;}

private:

    axis_params parameters;

};


#endif /* end of include guard:  */
