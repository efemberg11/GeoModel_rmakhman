
#ifndef fractionHandler_H
#define fractionHandler_H

// ancillary handler to element creation

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/fraction.h"
class fractionHandler:public GDMLHandler {
public:
	fractionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		f.ref=getAttributeAsString("ref");
		f.weight=getAttributeAsDouble("n");
	}
	fraction f;
	fraction getFraction() {return f;}
};


#endif /* end of include guard:  */
