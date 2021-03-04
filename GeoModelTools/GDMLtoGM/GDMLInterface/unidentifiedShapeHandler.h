
#ifndef unidentifiedShapeHandler_H
#define unidentifiedShapeHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class unidentifiedShapeHandler:public GDMLHandler {
public:
	unidentifiedShapeHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
