
#ifndef constantHandler_H
#define constantHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class constantHandler:public GDMLHandler {
public:
	constantHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
