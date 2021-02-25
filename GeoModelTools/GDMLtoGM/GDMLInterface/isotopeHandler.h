
#ifndef isotopeHandler_H
#define isotopeHandler_H

// ancillary handler to element creation

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/isotope.h"

class isotopeHandler:public GDMLHandler {
public:
  	std::string symbol(std::string s);
	isotopeHandler(std::string n, GDMLController* c);
	void ElementHandle();
	void postLoopHandling();
	void setAtomicWeight(double d);
private:
	isotope i;
};


#endif /* end of include guard:  */
