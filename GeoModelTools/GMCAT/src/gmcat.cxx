/*
 *   Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration   
*/

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModelWrite/WriteGeoModel.h"

#include "GeoModelKernel/GeoGeometryPluginLoader.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoPVLink.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPublisher.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>

#define SYSTEM_OF_UNITS GeoModelKernelUnits // --> 'GeoModelKernelUnits::cm'
#ifdef __APPLE__
const std::string shared_obj_extension=".dylib";
#else
const std::string shared_obj_extension=".so";
#endif


void publishMetaData( GMDBManager & db,
		      std::vector<std::string> &inputFiles,
		      std::vector<std::string> &inputPlugins,
		      std::string              &outputFile);

int main(int argc, char ** argv) {



  //
  // Usage message:
  //

  std::string gmcat= argv[0];
  std::string usage= "usage: " + gmcat + " [plugin1"+shared_obj_extension
    + "] [plugin2" + shared_obj_extension
    + "] ...[file1.db] [file2.db].. -o outputFile]";
  //
  // Print usage message if no args given:
  //
  if (argc==1) {
    std::cerr << usage << std::endl;
    return 0;
  }
  //
  // Parse the command line:
  //
  std::vector<std::string> inputFiles;
  std::vector<std::string> inputPlugins;
  std::string outputFile;
  bool outputFileSet = false;
  for (int argi=1;argi<argc;argi++) {
      std::string argument=argv[argi];
      if (argument.find("-o")!=std::string::npos) {
          argi++;
          if (argi>=argc) {
              std::cerr << usage << std::endl;
              return 1;
          }
          outputFile=argv[argi];
          outputFileSet = true;
      }
      else if (argument.find("-v")!=std::string::npos) {
          setenv("GEOMODEL_GEOMODELIO_VERBOSE", "1", 1); // does overwrite
          std::cout << "You set the verbosity level to 1" << std::endl;
      }
      else if (argument.find(shared_obj_extension)!=std::string::npos) {
          inputPlugins.push_back(argument);
      }
      else if (argument.find(".db")!=std::string::npos) {
          inputFiles.push_back(argument);
      }
      else {
          std::cerr << "Unrecognized argument " << argument << std::endl;
          std::cerr << usage << std::endl;
          return 2;
      }
  }
  if( !outputFileSet ) {
      std::cerr << "\nERROR! You should set an output file.\n" << std::endl;
      std::cerr << usage << std::endl;
      return 3;
  }

  //
  // Check that we can access the output file
  //
  if (access(outputFile.c_str(),F_OK)==0) {
    if (!access(outputFile.c_str(),W_OK)) {
      if (system(("rm -f "+ outputFile).c_str())) {
	std::cerr << "gmcat -- Error, cannot overwrite existing file " << outputFile << std::endl;
	return 3;
      }
    }
    else {
      std::cerr << "gmcat -- Error, cannot overwrite existing file " << outputFile << " (permission denied)" << std::endl;
      return 4;
    }
  }


  //
  // Create elements and materials for the "World" volume, which is the container:
  //

  const double  gr =   SYSTEM_OF_UNITS::gram;
  const double  mole = SYSTEM_OF_UNITS::mole;
  const double  cm3 =  SYSTEM_OF_UNITS::cm3;

  // Define the chemical elements
  GeoElement*  Nitrogen = new GeoElement ("Nitrogen" ,"N"  ,  7.0 ,  14.0067 *gr/mole);
  GeoElement*  Oxygen   = new GeoElement ("Oxygen"   ,"O"  ,  8.0 ,  15.9995 *gr/mole);
  GeoElement*  Argon    = new GeoElement ("Argon"    ,"Ar" , 18.0 ,  39.948  *gr/mole);
  GeoElement*  Hydrogen = new GeoElement ("Hydrogen" ,"H"  ,  1.0 ,  1.00797 *gr/mole);

  double densityOfAir=0.001214 *gr/cm3;
  GeoMaterial *air = new GeoMaterial("Air", densityOfAir);
  air->add(Nitrogen  , 0.7494);
  air->add(Oxygen, 0.2369);
  air->add(Argon, 0.0129);
  air->add(Hydrogen, 0.0008);
  air->lock();

  //
  // Create a huge world volume made of Air:
  //

  const GeoBox* worldBox = new GeoBox(2000*SYSTEM_OF_UNITS::cm, 2000*SYSTEM_OF_UNITS::cm, 2500*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
  GeoPhysVol *world=new GeoPhysVol(worldLog);
  world->ref();
  //
  // Loop over plugins, create the geometry and put it under the world:
  //
  std::vector<GeoPublisher*> vecPluginsPublishers; // caches the stores from all plugins
  for (const std::string & plugin : inputPlugins) {
    GeoGeometryPluginLoader loader;
    GeoVGeometryPlugin *factory=loader.load(plugin);
    if (!factory) {
      std::cerr << "gmcat -- Could not load plugin " << plugin << std::endl;
      return 5;
    }
    
    // NOTE: we want the plugin to publish lits FPV and AXF nodes, 
    // if it is intended to do that, and store them in the DB. 
    // For that, we pass a true to the plugin's `create()` method, 
    // which will use it to publish nodes, 
    // and then we get from the plugin the pointer to the GeoPublisher instance 
    // and we cache it for later, to dump the published nodes into the DB.
    factory->create(world, true);
    if( nullptr != factory->getPublisher() ) {
        vecPluginsPublishers.push_back( factory->getPublisher() ); // cache the publisher, if any, for later
    }
  }

  //
  // Loop over files, create the geometry and put it under the world:
  //
  for (const std::string & file : inputFiles) {
    GMDBManager* db = new GMDBManager(file);
    if (!db->checkIsDBOpen()){
      std::cerr << "gmcat -- Error opening the input file: " << file << std::endl;
      return 6;
    }

    /* set the GeoModel reader */
    GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);

    /* build the GeoModel geometry */
    GeoPhysVol* dbPhys = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory

    /* get an handle on a Volume Cursor, to traverse the whole set of Volumes */
    GeoVolumeCursor aV(dbPhys);

    /* loop over the Volumes in the tree */
    while (!aV.atEnd()) {

	if (aV.getName()!="ANON") {
	  GeoNameTag *nameTag=new GeoNameTag(aV.getName());
	  world->add(nameTag);
	}
	GeoTransform *transform= new GeoTransform(aV.getTransform());
	world->add(transform);
	world->add((GeoVPhysVol *) &*aV.getVolume());
	aV.next();
    }

    delete db;
  }
  //
  // Open a new database:
  //
  GMDBManager db(outputFile);
  //
  // check the DB connection
  //
  if (!db.checkIsDBOpen()) {
    std::cerr << "gmcat -- Error opening the output file: " << outputFile << std::endl;
    return 7;
  }

  GeoModelIO::WriteGeoModel dumpGeoModelGraph(db);
  world->exec(&dumpGeoModelGraph);

  if (vecPluginsPublishers.size() > 0) {
    dumpGeoModelGraph.saveToDB(vecPluginsPublishers);
  } else {
    dumpGeoModelGraph.saveToDB();
  }

  world->unref();

  publishMetaData(db,inputFiles,inputPlugins,outputFile);
  
  return 0;
}
