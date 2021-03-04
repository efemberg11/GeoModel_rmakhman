#include "GDMLInterface/replicaHandler.h"

#include "GDMLInterface/GDMLController.h"

#include "GDMLInterface/volumerefHandler.h"
#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"

#include "GDMLInterface/replicate_axisHandler.h"

replicaHandler::replicaHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
	new replicate_axisHandler("replicate_along_axis",c);
} 
void replicaHandler::ElementHandle()
{
	std::cout<<"this is replicaHandler::ElementHandle() "<<std::endl;
	nCopies=getAttributeAsInt("number");



}
GeoPhysVol* replicaHandler::getPhysicalVolume(int i)
{
	return thePhysicalVolumes[i];
}
GeoTransform* replicaHandler::getTransform(int i) 
{
	return transforms[i];
}
void replicaHandler::postLoopHandling()
{
}
int replicaHandler::getNCopies()
{
	return nCopies;
}
