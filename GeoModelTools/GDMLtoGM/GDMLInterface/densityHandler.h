
#ifndef densityHandler_H
#define densityHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class densityHandler:public GDMLHandler {
public:

	densityHandler(std::string n, GDMLController* c);
	void ElementHandle();
	double getDensity();

private:
	double unit;
	double value;
	double density;
};


#endif /* end of include guard:  */
