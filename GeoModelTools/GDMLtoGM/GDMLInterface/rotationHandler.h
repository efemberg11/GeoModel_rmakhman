
#ifndef rotationHandler_H
#define rotationHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/rotation.h"

class GDMLController;

class rotationHandler:public GDMLHandler {
public:
	rotationHandler(std::string n, GDMLController* c);
	void ElementHandle();
	rotation& getRotation();
	rotation p;
};


#endif /* end of include guard:  */
