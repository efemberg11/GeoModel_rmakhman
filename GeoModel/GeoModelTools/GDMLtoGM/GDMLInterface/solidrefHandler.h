
#ifndef solidrefHandler_H
#define solidrefHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoShape.h"

class solidrefHandler:public GDMLHandler {
public:

	solidrefHandler(std::string , GDMLController*);
	void ElementHandle();
	GeoShape* getSolid();
private:
	std::string ref="";
	GeoShape* theShape=0;
};


#endif /* end of include guard:  */
