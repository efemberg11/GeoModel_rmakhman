
#ifndef booleanHandler_H
#define booleanHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoShape.h"

class booleanHandler:public GDMLHandler {
public:

	booleanHandler(std::string , GDMLController*);
	void ElementHandle();
	GeoShape* getSolid();
private:
	std::string ref="";
	GeoShape* theShape=0;
};


#endif /* end of include guard:  */
