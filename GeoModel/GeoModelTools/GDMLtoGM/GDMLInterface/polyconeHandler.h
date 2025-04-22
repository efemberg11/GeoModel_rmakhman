
#ifndef polyconeHandler_H
#define polyconeHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class polyconeHandler:public GDMLHandler {
public:
	polyconeHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
