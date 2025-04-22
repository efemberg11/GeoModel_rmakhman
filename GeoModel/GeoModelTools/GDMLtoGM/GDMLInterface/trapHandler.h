
#ifndef trapHandler_H
#define trapHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class trapHandler:public GDMLHandler {
public:
	trapHandler(std::string, GDMLController*);
	void ElementHandle();

};


#endif /* end of include guard:  */
