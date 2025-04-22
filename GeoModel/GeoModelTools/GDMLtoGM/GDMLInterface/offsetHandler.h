
#ifndef offsetHandler_H
#define offsetHandler_H

// 

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class offsetHandler:public GDMLHandler {
public:
	offsetHandler(std::string n, GDMLController* c);
	void ElementHandle();
	double getOffset();
private:
	double unit;
	double value;
	double offset;
};


#endif /* end of include guard:  */
