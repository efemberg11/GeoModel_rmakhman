
#ifndef solidsHandler_H
#define solidsHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class solidsHandler:public GDMLHandler {
public:
	solidsHandler(std::string n, GDMLController* c):GDMLHandler(n,c) {}
	void ElementHandle() {;}
	void postLoopHandling() {;}

};


#endif /* end of include guard:  */
