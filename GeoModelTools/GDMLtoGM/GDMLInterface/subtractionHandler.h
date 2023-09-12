
#ifndef subtractionHandler_H
#define subtractionHandler_H

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLHandler.h"

class GDMLController;

class subtractionHandler:public GDMLHandler {
public:

	subtractionHandler(std::string n, GDMLController* c);
	void ElementHandle();
private:

};


#endif /* end of include guard:  */
