
#ifndef tubeHandler_H
#define tubeHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class tubeHandler:public GDMLHandler {
public:
  tubeHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
