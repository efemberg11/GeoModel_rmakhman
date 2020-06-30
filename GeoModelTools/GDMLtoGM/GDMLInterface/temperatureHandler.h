
#ifndef temperatureHandler_H
#define temperatureHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class temperatureHandler:public GDMLHandler {
public:
	temperatureHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {;}
};


#endif /* end of include guard:  */
