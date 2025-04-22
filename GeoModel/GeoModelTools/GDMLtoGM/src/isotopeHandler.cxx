#include "GDMLInterface/isotopeHandler.h"
#include <string>

std::string isotopeHandler::symbol(std::string s)
{
	if (isdigit(s[1])) return s.substr(0,1);
	else return s.substr(0,2);
}

isotopeHandler::isotopeHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void isotopeHandler::ElementHandle() 
{
	i.clear();
	i.ref=getAttributeAsString("name");
	i.name=stripPointer(i.ref);
	i.symbol=symbol(i.name);
	i.N=getAttributeAsInt("N");
	i.Z=getAttributeAsInt("Z");
}
void isotopeHandler::postLoopHandling()
{
	theController->saveIsotope(i.ref,i);
}
void isotopeHandler::setAtomicWeight(double d) 
{
	i.atomic_weight=d;
}
