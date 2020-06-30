/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"

#include "GeoModelXMLParser/XercesParser.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GDMLInterface/GDMLController.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

#include <iostream>

class GDMLtoGM : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  GDMLtoGM();

  // Destructor:
  ~GDMLtoGM();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, GeoVStore* store);

 private:

  // Illegal operations:
  const GDMLtoGM & operator=(const GDMLtoGM &right)=delete;
  GDMLtoGM(const GDMLtoGM &right) = delete;

};





GDMLtoGM::GDMLtoGM()
{
}


GDMLtoGM::~GDMLtoGM()
{
}


//## Other Operations (implementation)
void GDMLtoGM::create(GeoPhysVol *world, GeoVStore*)
{

  	std::cout<< "creating a GDMLController and the XercesParser"<<std::endl;
    GDMLController controller("GDMLController");
	  XercesParser xercesParser;
  	xercesParser.ParseFileAndNavigate("gdmlfile.xml");
	GeoPhysVol* w=controller.getWorld();
	GeoNameTag* gdmlTag=new GeoNameTag("GDML setup");
	world->add(gdmlTag);
	world->add(w);
}

extern "C" GDMLtoGM *createGDMLtoGM() {
  return new GDMLtoGM;
}
