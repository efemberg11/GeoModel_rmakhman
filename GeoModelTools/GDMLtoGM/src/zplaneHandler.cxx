#include "GDMLInterface/zplaneHandler.h"

#include "GDMLInterface/GDMLController.h"

zplaneHandler::zplaneHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {
}
void zplaneHandler::ElementHandle()
{
	//std::cout<<"handling zplane "<<std::endl;
	p.rmin=getAttributeAsDouble("rmin");
	p.rmax=getAttributeAsDouble("rmax");
	p.z=getAttributeAsDouble("z");
}
zPlane& zplaneHandler::getZPlane() 
{
	return p;
}
