
#ifndef scaleHandler_H
#define scaleHandler_H

#include "GeoModelKernel/GeoDefinitions.h"

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/rotation.h"

class scaleHandler:public GDMLHandler {
public:
	scaleHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		std::string hName=GetName();
		//std::cout<<" handler for scale "<<std::endl;
		if (hName=="scale")
		{
			p.name="";
			p.scalex=p.scaley=p.scalez=0;
			p.name=getAttributeAsString("name");
			p.scalex=getAttributeAsDouble("x");
			p.scaley=getAttributeAsDouble("y");
			p.scalez=getAttributeAsDouble("z");
			
			theController->saveScale(p.name,p);
		}
		else if (hName=="scaleref")
		{
			std::string ref=getAttributeAsString("ref");
			p=theController->retrieveScale(ref);
		}
	}
	scale& getScale() {return p;}
private:
	scale p;
};


#endif /* end of include guard:  */
