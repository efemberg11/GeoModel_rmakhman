
#ifndef physvolHandler_H
#define physvolHandler_H

#include "GDMLInterface/GDMLHandler.h"

#include <string>

class GeoPhysVol;
class GeoTransform;

class physvolHandler:public GDMLHandler {
public:

<<<<<<< HEAD
    
	physvolHandler(std::string n, GDMLController* c);
	void ElementHandle();
	GeoPhysVol* getPhysicalVolume();
	GeoTransform* getTransform();
=======
	physvolHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		thePhysicalVolume=0;
		//NB the attribute 'name' is not mandatory
                //better to use the getAttributeAsString with the boolean option
                bool isPresent;
		name=getAttributeAsString("name", isPresent);
//              if (isPresent)
//                 std::cout<<" this is physvolHandler: name "<<name<<std::endl;

		StopLoop(true);
		xercesc::DOMNode *child;
		
		//GeoTrf::TransformRT pVTransform=GeoTrf::Transform3D::Identity();
		GeoTrf::Transform3D pVRotation=GeoTrf::Transform3D::Identity();
		GeoTrf::Vector3D pVTranslation=GeoTrf::Vector3D(0,0,0);
                GeoTrf::Transform3D pVScale=GeoTrf::Scale3D::Identity();
>>>>>>> gdml2gm: handle properly optional parameted in physvol, position and rotation handlers


private:
	std::string name;
	GeoPhysVol* thePhysicalVolume=0;
	GeoTransform* trf=0;
};


#endif /* end of include guard:  */
