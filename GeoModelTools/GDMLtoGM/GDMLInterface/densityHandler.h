
#ifndef densityHandler_H
#define densityHandler_H

// this is (AFAICS) useless information on the GeoModel side. Therefore, this handler does nothing

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/materialHandler.h"
#include <string>
#include <iostream>

class densityHandler:public GDMLHandler {
public:
	densityHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		unit=value=density=0;
		unit=getAttributeAsDouble("unit",GeoModelKernelUnits::g/GeoModelKernelUnits::mole);
		value=getAttributeAsDouble("value");
		density=unit*value;
		// std::cout<<" this is densityHandler: density= "<<density<<std::endl;
	}
	double getDensity() {return density;}
private:
	double unit;
	double value;
	double density;
};


#endif /* end of include guard:  */
