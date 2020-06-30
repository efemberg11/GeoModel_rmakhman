
#ifndef volumeHandler_H
#define volumeHandler_H

#include <vector>

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/materialrefHandler.h"
#include "GDMLInterface/solidrefHandler.h"
#include "GDMLInterface/physvolHandler.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoPhysVol.h"

class volumeHandler:public GDMLHandler {
public:

	volumeHandler(std::string , GDMLController* );
	void ElementHandle();
	void postLoopHandling();
private:
	std::string name;
	GeoVolume theVolume;
	GeoMaterial* material;
	GeoShape* shape;

};


#endif /* end of include guard:  */
