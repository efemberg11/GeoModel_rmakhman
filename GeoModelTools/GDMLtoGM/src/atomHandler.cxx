#include "GDMLInterface/atomHandler.h"
#include "GeoModelKernel/Units.h"

atomHandler::atomHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void atomHandler::ElementHandle() {
	// std::cout <<" atomHandling handle "<<std::endl;
	unit=value=atomic_weight=0;
	unit=getAttributeAsDouble("unit", GeoModelKernelUnits::g/GeoModelKernelUnits::mole);
	value=getAttributeAsDouble("value");
	atomic_weight=unit*value;
}
void atomHandler::postLoopHandling()
{
	XMLHandler* h=theController->XMLStore()->GetHandler(s_currentElement->getParentNode());
	if (h->GetName() != "isotope") return;
	isotopeHandler* theParentHandler=dynamic_cast<isotopeHandler*> (h);
	if (theParentHandler) theParentHandler->setAtomicWeight(atomic_weight);
	//else std::cout<<" Help!!!! wrong type of Handler!!!!"<<std::endl;
}
double atomHandler::atomicWeight()
{
	return atomic_weight;
}
