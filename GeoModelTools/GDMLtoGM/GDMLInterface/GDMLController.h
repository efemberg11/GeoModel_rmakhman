#pragma once

#include <string>

#include "ExpressionEvaluator/ExpressionEvaluator.h"
#include "GeoModelXMLParser/XMLHandlerStore.h"

#include "GDMLInterface/isotope.h"
#include "GDMLInterface/position.h"
#include "GDMLInterface/rotation.h"
#include "GDMLInterface/scale.h"
#include "GDMLInterface/element.h"

class GeoShape;
class GeoElement;
class GeoMaterial;
class GeoLogVol;
class GeoPhysVol;

typedef std::pair<GeoLogVol*,GeoPhysVol* > GeoVolume;

typedef std::map<std::string, GeoShape*, std::less<std::string> > solidStore;

typedef std::map<std::string, GeoMaterial*, std::less<std::string> > materialStore;
typedef std::map<std::string, GeoVolume, std::less<std::string> > logicalVolumeStore;
typedef std::map<std::string, isotope, std::less<std::string> > isotopeStore;
typedef std::map<std::string, position, std::less<std::string> > positionStore;
typedef std::map<std::string, rotation, std::less<std::string> > rotationStore;
typedef std::map<std::string, scale, std::less<std::string> > scaleStore;
typedef std::map<std::string, GeoElement*, std::less<std::string> > elementStore;



class GDMLController {
public:
	GDMLController(std::string);
	ExpressionEvaluator* Evaluator();
	XMLHandlerStore* XMLStore();

	void saveIsotope(std::string, isotope);
	isotope& retrieveIsotope(std::string);

        void saveSolid(std::string, GeoShape*);
	GeoShape* retrieveSolid(std::string);

	void savePosition(std::string, position);
	position& retrievePosition(std::string);
	
	void saveRotation(std::string, rotation);
	rotation& retrieveRotation(std::string);
    
        void saveScale(std::string, scale);
        scale& retrieveScale(std::string);

	void saveElement(std::string, GeoElement*);
	GeoElement* retrieveElement(std::string);


	void saveMaterial(std::string, GeoMaterial*);
	GeoMaterial* retrieveMaterial(std::string);

	void saveLogicalVolume(std::string, GeoVolume);
	GeoVolume retrieveLogicalVolume(std::string);
	
	void setWorld(GeoVolume v) {world=v;}
	GeoPhysVol* getWorld() {return world.second;}

protected:
	void registerHandlers();
	void setEvaluatorUnits();
	void setConstant(const char*, double);

	std::string controllerName;
	ExpressionEvaluator* theEvaluator;
	XMLHandlerStore* theXMLStore;

	isotopeStore theIsotopes;
	positionStore thePositions;
	rotationStore theRotations;
        scaleStore theScales;
	elementStore theElements;

	solidStore theSolids;
	materialStore theMaterials;
	logicalVolumeStore theLogVolumes;
	GeoVolume world;
};
