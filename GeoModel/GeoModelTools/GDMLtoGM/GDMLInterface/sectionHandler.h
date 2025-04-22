
#ifndef sectionHandler_H
#define sectionHandler_H

#include "GDMLInterface/GDMLHandler.h"

class GDMLController;

struct section {
	double scalingFactor;
	double xOffset;
	double yOffset;
	int zOrder;
	double zPosition;
};

class sectionHandler:public GDMLHandler {
public:
	sectionHandler(std::string n, GDMLController* c);
	void ElementHandle();
	section& getSection();
private:
	section s;
};


#endif /* end of include guard:  */
