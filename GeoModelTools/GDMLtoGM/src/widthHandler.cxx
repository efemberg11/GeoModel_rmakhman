#include "GDMLInterface/widthHandler.h"
#include "GeoModelKernel/Units.h"

widthHandler::widthHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void widthHandler::ElementHandle() 
{
	unit=value=width=0;
	unit=getAttributeAsDouble("unit",GeoModelKernelUnits::g/GeoModelKernelUnits::mole);
	value=getAttributeAsDouble("value");
	width=unit*value;
	// std::cout<<" this is widthHandler: width= "<<width<<std::endl;
}
double widthHandler::getWidth() 
{
	return width;
}
