#include "GDMLInterface/offsetHandler.h"
#include "GeoModelKernel/Units.h"

offsetHandler::offsetHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
}
void offsetHandler::ElementHandle() 
{
	unit=value=offset=0;
	unit=getAttributeAsDouble("unit",GeoModelKernelUnits::g/GeoModelKernelUnits::mole);
	value=getAttributeAsDouble("value");
	offset=unit*value;
	// std::cout<<" this is offsetHandler: offset= "<<offset<<std::endl;
}
double offsetHandler::getOffset() 
{
	return offset;
}
