
#ifndef elementHandler_H
#define elementHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"


class elementHandler:public GDMLHandler {
public:
	elementHandler(std::string , GDMLController* );
	void ElementHandle();
	void postLoopHandling();
private:
	bool isAtom=false;
	std::string name="";
	std::string symbol="";
	element e;
	int Z=0;
	double weight;
};


#endif /* end of include guard:  */
