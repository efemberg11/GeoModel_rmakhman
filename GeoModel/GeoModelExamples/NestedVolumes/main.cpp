// Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration

/*
 *  NestedVolumes
 *
 *  Author:     Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *  Created on: Aug 2021
 *
 *  An example, showing how to implement a simple geometry featuring nested
 *  and shared volumes.
 *
 *  We want to implement a big 'mother' box, 
 *  which contains three smaller 'daughter' boxes.
 *  The three 'daughter' boxes will be identical, 
 *  so we only need to create one instance of them,
 *  and then use that for all the three occurrences:
 *  the volumes will be created only once, then shared; 
 *  that will save memory
 *
 */

// GeoModel includes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoTransform.h"

#include "GeoModelDBManager/GMDBManager.h"

#include "GeoModelWrite/WriteGeoModel.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'


int main(int argc, char *argv[])
{
  //-----------------------------------------------------------------------------------//
	// Define the materials that we shall use.                                              //
	// ----------------------------------------------------------------------------------//

	// Define the units
	#define gr   SYSTEM_OF_UNITS::gram
	#define mole SYSTEM_OF_UNITS::mole
	#define cm3  SYSTEM_OF_UNITS::cm3

	// Define the chemical elements
	GeoElement*  Nitrogen = new GeoElement ("Nitrogen" ,"N"  ,  7.0 ,  14.0067 *gr/mole);
	GeoElement*  Oxygen   = new GeoElement ("Oxygen"   ,"O"  ,  8.0 ,  15.9995 *gr/mole);
	GeoElement*  Argon    = new GeoElement ("Argon"    ,"Ar" , 18.0 ,  39.948  *gr/mole);
	GeoElement*  Hydrogen = new GeoElement ("Hydrogen" ,"H"  ,  1.0 ,  1.00797 *gr/mole);
	GeoElement*  Iron     = new GeoElement ("Iron"     ,"Fe" , 26.0 ,  55.847  *gr/mole);
	GeoElement*  Carbon   = new GeoElement ("Carbon"   ,"C"  ,  6.0 ,  12.0107 *gr/mole);

	// Define the materials

	double densityOfAir=0.001214 *gr/cm3;
	GeoMaterial *air = new GeoMaterial("Air", densityOfAir);
	air->add(Nitrogen  , 0.7494);
	air->add(Oxygen, 0.2369);
	air->add(Argon, 0.0129);
	air->add(Hydrogen, 0.0008);
	air->lock();

	GeoMaterial* steel  = new GeoMaterial("Steel", 7.9 *gr/cm3);
	steel->add(Iron  , 0.98);
	steel->add(Carbon, 0.02);
	steel->lock();


  //-----------------------------------------------------------------------------------//
  // create the world volume container and
  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Creating the 'world' volume, i.e. the root volume of the GeoModel tree..." << std::endl;
  const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
  GeoPhysVol* world = new GeoPhysVol(worldLog);


	//-----------------------------------------------------------------------------------//
	// Now, we implement the shapes and the volumes and we use them.
    // Remember, we want to implement a big 'mother' box, 
    // which contains three smaller 'daughter' boxes.
    // The three 'daughter' boxes will be identical, 
    // so we only need to create one instance of them,
    // and then use that for all the three occurrences:
    // the volumes will be created only once, then shared; 
    // that will save memory

    // Make the box that describes the shape of the 'mother' volume                    //
	const GeoBox      *motherBox    = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);                   //
	// Bundle this with a material into a logical volume:                                //
	const GeoLogVol   *motherLog    = new GeoLogVol("MotherVolume", motherBox, air);                 //
	// ..And create a physical volume:                                                   //
	GeoPhysVol        *motherPhys   = new GeoPhysVol(motherLog);                               //


    // Now, do the same for the 'daughter' volume
    // Note: we only need to define one Shape, one LogVol, and one PhysVol
    // even if we use that three times for three different placed volumes
    // they will be all shared
	GeoBox       *daughterBox  = new GeoBox(50*SYSTEM_OF_UNITS::cm, 50*SYSTEM_OF_UNITS::cm, 50*SYSTEM_OF_UNITS::cm);
	GeoLogVol    *daughterLog  = new GeoLogVol("SingleBox", daughterBox, air);
	GeoPhysVol   *daughterPhys = new GeoPhysVol(daughterLog);

    // Create two transformations, to place two of the three 'daughters'
    GeoTrf::Translate3D Tr1(-500*SYSTEM_OF_UNITS::cm, 0, 0); // translation to the left
	GeoTrf::Translate3D Tr2( 500*SYSTEM_OF_UNITS::cm, 0, 0); // translation to the right

	GeoTransform* trLeft = new GeoTransform(Tr1);
	GeoTransform* trRight = new GeoTransform(Tr2);

    // Add the three 'daughter' volumes to the 'mother' volume:
	motherPhys->add(daughterPhys); // center 'daughter'
    motherPhys->add(trLeft);
	motherPhys->add(daughterPhys); // left 'daughter'
    motherPhys->add(trRight);
	motherPhys->add(daughterPhys); // right 'daughter'

	//------------------------------------------------------------------------------------//
	// Now insert a NameTag into the world
    // This will name the root volume, and it will be displayed by GMEX (GeoModelExplorer)
    // as the name of the branch (i.e., opening the item designated with this name, 
    // it will show the geometry we are implementing here).
    GeoNameTag *tag = new GeoNameTag("ThreeBoxes");
    world->add(tag);
	
    //------------------------------------------------------------------------------------//
	// Now insert the 'mother' box volume into the world...                                           //
    world->add(motherPhys);
	//------------------------------------------------------------------------------------//




	//------------------------------------------------------------------------------------//
	// Writing the geometry to file
	//------------------------------------------------------------------------------------//
  std::string path = "geometry.db";

	// check if DB file exists. If not, return.
  // FIXME: TODO: this check should go in the 'GMDBManager' constructor.
  std::ifstream infile(path.c_str());
    if ( infile.good() ) {
      std::cout << "\n\tERROR!! A '" << path << "' file exists already!! Please, remove, move, or rename it before running this program. Exiting...";
        exit(EXIT_FAILURE);
  }
  infile.close();

	// open the DB connection
  GMDBManager db(path);

  // check the DB connection
  if (db.checkIsDBOpen()) {
    std::cout << "OK! Database is open!" << std::endl;
  } else {
    std::cout << "Database ERROR!! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Dump the tree volumes to a local file
	std::cout << "Dumping the GeoModel geometry to the DB file..." << std::endl;
  GeoModelIO::WriteGeoModel dumpGeoModelGraph(db); // init the GeoModel node action
  world->exec(&dumpGeoModelGraph); // visit all GeoModel nodes
  dumpGeoModelGraph.saveToDB(); // save to the SQlite DB file
  std::cout << "DONE. Geometry saved." <<std::endl;

  std::cout << "\nTest - list of all the GeoMaterial nodes in the persistified geometry:" << std::endl;
  db.printAllMaterials();
  std::cout << "\nTest - list of all the GeoElement nodes in the persistified geometry:" << std::endl;
  db.printAllElements();


  return 0;
}
