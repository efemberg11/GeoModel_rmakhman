#ifndef atomHandler_H
#define atomHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/isotopeHandler.h"
#include <string>
#include <iostream>

class atomHandler:public GDMLHandler {
public:
	atomHandler(std::string n, GDMLController* c);
	void ElementHandle();
	void postLoopHandling();
	double atomicWeight();
private:
	double unit;
	double value;
	double atomic_weight;
};


#endif /* end of include guard:  */
