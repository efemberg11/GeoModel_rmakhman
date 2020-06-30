
#ifndef trdHandler_H
#define trdHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class trdHandler:public GDMLHandler {
public:
	trdHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
