
#ifndef volumerefHandler_H
#define volumerefHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoLogVol.h"

class volumerefHandler:public GDMLHandler {
public:

	volumerefHandler(std::string, GDMLController*);
	void ElementHandle();
	GeoVolume getVolume();
private:
	std::string ref="";
	GeoVolume theVolume;
};


#endif /* end of include guard:  */
