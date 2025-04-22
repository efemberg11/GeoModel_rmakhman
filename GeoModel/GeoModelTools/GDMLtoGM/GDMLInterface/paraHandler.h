
#ifndef paraHandler_H
#define paraHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class paraHandler:public GDMLHandler {
public:
	paraHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
