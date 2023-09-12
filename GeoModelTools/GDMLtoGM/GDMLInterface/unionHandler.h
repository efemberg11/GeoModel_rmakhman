
#ifndef unionHandler_H
#define unionHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include <string>

class GDMLController;

class unionHandler:public GDMLHandler {
public:

	unionHandler(std::string n, GDMLController* c);
	void ElementHandle();
private:

};


#endif /* end of include guard:  */
