
#ifndef coneHandler_H
#define coneHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class coneHandler:public GDMLHandler {
public:
	coneHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
