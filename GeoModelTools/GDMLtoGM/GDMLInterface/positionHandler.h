
#ifndef positionHandler_H
#define positionHandler_H

// ancillary handler to element creation

#include "GDMLInterface/GDMLHandler.h"

#include "GDMLInterface/position.h"

#include <string>

class GDMLController;

class positionHandler:public GDMLHandler {
public:

	positionHandler(std::string n, GDMLController* c);
	void ElementHandle();
	position& getPosition();
private:
	position p;
};


#endif /* end of include guard:  */
