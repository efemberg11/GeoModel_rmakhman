
#pragma once

#include "GDMLInterface/GDMLHandler.h"

class GDMLController;

#include <string>
#include <vector>

class GeoPhysVol;
class GeoSerialTransformer;
class GeoLogVol;

class replicaHandler:public GDMLHandler {
public:

	replicaHandler(std::string n, GDMLController* c);
	void ElementHandle();
	GeoPhysVol* getPhysicalVolume();
	GeoSerialTransformer* getTransform();
	int getNCopies();
	void postLoopHandling();
	void setPhysicalVolume(GeoPhysVol* pV) {thePhysicalVolume=pV;}
	void setMotherVolume(GeoPhysVol* lV) {theMotherVolume=lV;}
private:
	std::string name;
	int nCopies=0;
	GeoPhysVol* thePhysicalVolume;
	GeoPhysVol* theMotherVolume;
	GeoSerialTransformer* transform;
};


/* end of include guard:  */
