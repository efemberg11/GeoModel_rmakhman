
#ifndef MEEHandler_H
#define MEEHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class MEEHandler:public GDMLHandler {
public:
	MEEHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {;}
};


#endif /* end of include guard:  */
