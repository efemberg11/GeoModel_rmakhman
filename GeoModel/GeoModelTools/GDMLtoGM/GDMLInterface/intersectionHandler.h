
#ifndef intersectionHandler_H
#define intersectionHandler_H

#include "GDMLInterface/GDMLHandler.h"

class GDMLController;

class intersectionHandler:public GDMLHandler {
public:

	intersectionHandler(std::string n, GDMLController* c);
	void ElementHandle();
private:

};


#endif /* end of include guard:  */
