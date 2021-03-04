
#pragma once

#include "GDMLInterface/GDMLHandler.h"

class GDMLController;

#include <string>
#include <vector>

class GeoPhysVol;
class GeoTransform;

class replicaHandler:public GDMLHandler {
public:

	replicaHandler(std::string n, GDMLController* c);
	void ElementHandle();
	GeoPhysVol* getPhysicalVolume(int);
	GeoTransform* getTransform(int);
	int getNCopies();
	void postLoopHandling();
private:
	std::string name;
	int nCopies=0;
	std::vector<GeoPhysVol*> thePhysicalVolumes;
	std::vector<GeoTransform*> transforms;
};


/* end of include guard:  */
