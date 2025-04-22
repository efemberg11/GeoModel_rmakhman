
#ifndef physvolHandler_H
#define physvolHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include <string>

class GeoPhysVol;
class GeoTransform;

class physvolHandler:public GDMLHandler {
public:
    
	physvolHandler(std::string n, GDMLController* c);
	void ElementHandle();
	GeoPhysVol* getPhysicalVolume();
	GeoTransform* getTransform();
	void setPhysicalVolume(GeoPhysVol* pV) {thePhysicalVolume=pV;}
	void setMotherVolume(GeoPhysVol* lV) {theMotherVolume=lV;}
private:
	std::string name;
	GeoPhysVol* thePhysicalVolume=0;
	GeoPhysVol* theMotherVolume=0;
	GeoTransform* trf=0;
};


#endif /* end of include guard:  */
