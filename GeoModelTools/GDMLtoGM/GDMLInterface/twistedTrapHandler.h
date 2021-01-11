
#ifndef twistedTrapHandler_H
#define twistedTrapHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class twistedTrapHandler:public GDMLHandler {
public:
	twistedTrapHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
