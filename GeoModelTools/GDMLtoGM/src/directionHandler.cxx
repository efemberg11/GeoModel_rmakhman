#include "GDMLInterface/directionHandler.h"
#include "GDMLInterface/Axis.h"

directionHandler::directionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void directionHandler::ElementHandle() 
{
	int value=0;
	if ((value=getAttributeAsDouble("phi",0))) axis=PHI_axis;
	else if ((value=getAttributeAsDouble("x",0))) axis=X_axis;
	else if ((value=getAttributeAsDouble("y",0))) axis=Y_axis;
	else if ((value=getAttributeAsDouble("z",0))) axis=Z_axis;
}
Axis directionHandler::getAxis() 
{
	return axis;
}
