
#ifndef solidsHandler_H
#define solidsHandler_H

#include "GDMLInterface/GDMLHandler.h"


#include <string>

class GDMLController;

class solidsHandler:public GDMLHandler {
public:
	solidsHandler(std::string n, GDMLController* c);
	void ElementHandle();
	void postLoopHandling();

};


#endif /* end of include guard:  */
