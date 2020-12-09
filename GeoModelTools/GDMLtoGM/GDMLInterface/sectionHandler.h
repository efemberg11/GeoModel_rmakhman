
#ifndef sectionHandler_H
#define sectionHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

struct section {
	double scalingFactor;
	double xOffset;
	double yOffset;
	int zOrder;
	double zPosition;
};

class sectionHandler:public GDMLHandler {
public:
	sectionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		s.scalingFactor=getAttributeAsDouble("scalingFactor");
		s.xOffset=getAttributeAsDouble("xOffset");
		s.yOffset=getAttributeAsDouble("yOffset");
		s.zOrder=getAttributeAsInt("zOrder");
		s.zPosition=getAttributeAsDouble("zPosition");
	}
	section& getSection() {return s;}
private:
	section s;
};


#endif /* end of include guard:  */
