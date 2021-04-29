
#ifndef variableHandler_H
#define variableHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class variableHandler:public GDMLHandler {
public:
	variableHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
