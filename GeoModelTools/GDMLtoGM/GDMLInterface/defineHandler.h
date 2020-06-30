
#ifndef defineHandler_H
#define defineHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class defineHandler:public GDMLHandler {
public:
	defineHandler(std::string n, GDMLController* c);
	void ElementHandle() ;

};


#endif /* end of include guard:  */
