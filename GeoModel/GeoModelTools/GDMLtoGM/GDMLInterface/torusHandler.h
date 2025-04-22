
#ifndef torusHandler_H
#define torusHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class torusHandler:public GDMLHandler {
public:
	torusHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
