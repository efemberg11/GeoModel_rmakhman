#include "GDMLInterface/densityHandler.h"
#include "GeoModelKernel/Units.h"

densityHandler::densityHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void densityHandler::ElementHandle() 
{
	unit=value=density=0;
	unit=getAttributeAsDouble("unit",GeoModelKernelUnits::g/GeoModelKernelUnits::mole);
	value=getAttributeAsDouble("value");
	density=unit*value;
	// std::cout<<" this is densityHandler: density= "<<density<<std::endl;
}
double densityHandler::getDensity() 
{
	return density;
}
