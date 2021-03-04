
#ifndef directionHandler_H
#define directionHandler_H

// 

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/Axis.h"

#include <string>
#include <iostream>

class directionHandler:public GDMLHandler {
public:
	directionHandler(std::string n, GDMLController* c);
	void ElementHandle();
	Axis getAxis();
private:
	Axis axis;
};


#endif /* end of include guard:  */
