
#ifndef arb8Handler_H
#define arb8Handler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class arb8Handler:public GDMLHandler {
public:
	arb8Handler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
