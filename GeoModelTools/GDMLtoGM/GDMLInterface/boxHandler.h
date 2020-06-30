
#ifndef boxHandler_H
#define boxHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class boxHandler:public GDMLHandler {
public:
	boxHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
