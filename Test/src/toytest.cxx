/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoGeometryPluginLoader.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"

#include "GeoXmlMatManager/GeoXmlMatManager.h"

#include <iostream>

#define SYSTEM_OF_UNITS GeoModelKernelUnits

int main(int argc, char ** argv) {
  // Parse command line
  std::string strUsage{argv[0]};
  if(argc!=2) {
    strUsage = "Usage: " + strUsage + " <plugin_library>";
    std::cerr << strUsage << std::endl;
    return 1;
  }

  // Load Toy Plugin
  GeoGeometryPluginLoader loader;
  GeoVGeometryPlugin *factory=loader.load(argv[1]);
  if(!factory) {
    std::cerr << "Could not load plugin " << argv[1] << std::endl;
    return 1;
  }

  // Access XML Material Manager
  const GeoXmlMatManager* matman = GeoXmlMatManager::getManager();
  const GeoMaterial* air = matman->getMaterial("std::Air");

  // Build the container PhysVol
  const GeoBox* worldBox = new GeoBox(2000*SYSTEM_OF_UNITS::cm
				      , 2000*SYSTEM_OF_UNITS::cm
				      , 2000*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
  GeoPhysVol *world=new GeoPhysVol(worldLog);
  world->ref();

  std::cout << "Building Toy Geometry ... ";
  factory->create(world);

  // Clean up and declare success
  world->unref();  
  std::cout << " OK" << std::endl;
  return 0;
}
