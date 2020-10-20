/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"


#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

#include "GeoModelXml/GmxInterface.h"
#include "GeoModelXml/Gmx2Geo.h"

#include <iostream>
#include <fstream>
#include <sstream>

class GMXPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  GMXPlugin();

  // Destructor:
  ~GMXPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish);

 private:

  // Illegal operations:
  const GMXPlugin & operator=(const GMXPlugin &right)=delete;
  GMXPlugin(const GMXPlugin &right) = delete;

};





GMXPlugin::GMXPlugin()
{
}


GMXPlugin::~GMXPlugin()
{
}


//## Other Operations (implementation)
void GMXPlugin::create(GeoPhysVol *world, bool publish)
{

  	std::cout<< "creating a GeoModelXml detector "<<std::endl;
	
	std::ifstream xmlFile("gmx.xml");
	std::stringstream gmxInput;
	gmxInput<<xmlFile.rdbuf();
	
//	std::cout<< " string being parsed "<<std::endl<<std::endl<<gmxInput.str()<<std::endl;
	
	GmxInterface gmxInterface;
	Gmx2Geo gmx2Geo("gmx.xml", world, gmxInterface, 0);

}

extern "C" GMXPlugin *createGMXPlugin() {
  return new GMXPlugin;
}
