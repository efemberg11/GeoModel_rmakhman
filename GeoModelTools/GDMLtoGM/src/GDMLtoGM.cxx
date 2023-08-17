/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/
#include <fstream>

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelXMLParser/XercesParser.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GDMLInterface/GDMLController.h"

#include <iostream>

#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

inline bool exists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

class GDMLtoGM : public GeoVGeometryPlugin  {

 public:

  // Constructor:  
  GDMLtoGM() {}

  // Destructor:
  ~GDMLtoGM();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish = false ) override;

 private:

  // Illegal operations:
  const GDMLtoGM & operator=(const GDMLtoGM &right)=delete;
  GDMLtoGM(const GDMLtoGM &right) = delete;

};


GDMLtoGM::~GDMLtoGM()
{}


//## Other Operations (implementation)
void GDMLtoGM::create(GeoVPhysVol *world, bool /* 'publish' is not used here */)
{
	char* fPath=getenv("GDML_FILE_NAME");
	std::string fileName;
	if (fPath!=NULL) fileName=std::string(fPath);
	else fileName="gdmlfile.xml";
	if (!exists(fileName)) {
		std::cout <<"GDMLtoGeo: input file "<<fileName<<" does not exist. quitting and returning nicely! "<<std::endl;
		return;
	}
	
  	std::cout<< "GDMLtoGeo: GDML to GeoModel Interface. Parsing gdml file "<<fileName<<" and setting world volume"<<std::endl;
	GDMLController controller("GDMLController");
	std::cout<<"controller created, now parsing "<<std::endl;
	GeoModelTools::XercesParser xercesParser;
  	xercesParser.ParseFileAndNavigate(fileName);
	std::cout << "done parsing "<<std::endl;
	GeoPhysVol* w=controller.getWorld();
	const std::string nameTag=stripPointer(w->getLogVol()->getName());
	//std::cout<< "GDMLtoGeo: setting top volume name to: "<<nameTag<<std::endl;
	GeoNameTag* gdmlTag=new GeoNameTag(nameTag);
	world->add(gdmlTag);
	world->add(w);
}

extern "C" GDMLtoGM *createGDMLtoGM() {
  return new GDMLtoGM();
}
