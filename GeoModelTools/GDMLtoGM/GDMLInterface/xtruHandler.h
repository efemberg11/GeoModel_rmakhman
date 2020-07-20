
#ifndef xtruHandler_H
#define xtruHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class xtruHandler:public GDMLHandler {
public:
	xtruHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
