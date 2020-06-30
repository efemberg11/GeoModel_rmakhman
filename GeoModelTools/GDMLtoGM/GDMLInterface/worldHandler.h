
#ifndef worldHandler_H
#define worldHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

class worldHandler:public GDMLHandler {
public:

	worldHandler(std::string, GDMLController*);
	void ElementHandle();
private:
	std::string ref="";
};


#endif /* end of include guard:  */
