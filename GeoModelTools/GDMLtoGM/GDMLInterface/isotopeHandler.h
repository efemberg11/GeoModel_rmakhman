
#ifndef isotopeHandler_H
#define isotopeHandler_H

// ancillary handler to element creation

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/isotope.h"
#include "GDMLInterface/elementHandler.h"

class isotopeHandler:public GDMLHandler {
public:
  std::string symbol(std::string s)
	{
	  if (isdigit(s[1])) return s.substr(0,1);
		else return s.substr(0,2);
	}

	isotopeHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		i.clear();
		i.ref=getAttributeAsString("name");
		i.name=stripPointer(i.ref);
		i.symbol=symbol(i.name);
		i.N=getAttributeAsInt("N");
		i.Z=getAttributeAsInt("Z");
	}
	void postLoopHandling()
	{
    theController->saveIsotope(i.ref,i);
	}
	void setAtomicWeight(double d) {i.atomic_weight=d;}
private:
	isotope i;
};


#endif /* end of include guard:  */
