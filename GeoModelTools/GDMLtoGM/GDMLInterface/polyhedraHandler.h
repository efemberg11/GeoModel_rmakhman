
#ifndef polyhedraHandler_H
#define polyhedraHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class polyhedraHandler:public GDMLHandler {
public:
	polyhedraHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
