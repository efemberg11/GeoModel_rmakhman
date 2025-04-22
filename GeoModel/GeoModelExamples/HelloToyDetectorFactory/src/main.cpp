// Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

/*
 * HelloGeo.cpp
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Nov, 2018
 *
 */

// local includes
#include "GeoModelExperiment.h"
#include "ToyDetectorFactory.h"

// GeoModel includes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelWrite/WriteGeoModel.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'


GeoModelExperiment* createTheExperiment(GeoPhysVol* world = nullptr)
{
  if (world == nullptr)
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

    // Define the materials
    double densityOfAir=0.001214 *gr/cm3;
    GeoMaterial *air = new GeoMaterial("Air", densityOfAir);
    air->add(Nitrogen  , 0.7494);
    air->add(Oxygen, 0.2369);
    air->add(Argon, 0.0129);
    air->add(Hydrogen, 0.0008);
    air->lock();

    // Setup the 'World' volume from which everything else will be suspended
    const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
    const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
    world = new GeoPhysVol(worldLog);
  }
  // Setup the 'Experiment' manager
  GeoModelExperiment* theExperiment = new GeoModelExperiment(world);
  return theExperiment;
}



int main(int argc, char *argv[])
{

  //--------------------------------------------//
  // create the world volume container and its manager
  GeoModelExperiment* theExperiment = createTheExperiment();
  GeoPhysVol* world = theExperiment->getPhysVol();

  //--------------------------------------------//
  // we create the 'detector' geometry and we add it to the world volume
  ToyDetectorFactory factory; // more complex geometry example

  factory.create(world);
  std::cout << "treetop numbers: " << factory.getDetectorManager()->getNumTreeTops() << std::endl;

  std::cout << "DONE. Geometry created." << std::endl;


  //--------------------------------------------//
  // --- testing the newly created Geometry --- //

  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Getting the 'world' GeoPhysVol, i.e. the root volume of the GeoModel tree" << std::endl;
  GeoPhysVol* worldPtr = theExperiment->getPhysVol();

  // get the 'world' GeoLogVol
  std::cout << "Getting the GeoLogVol used by the 'world' volume" << std::endl;
  const GeoLogVol* logVol = worldPtr->getLogVol();
  std::cout << "'world' GeoLogVol name: " << logVol->getName() << std::endl;
  std::cout << "'world' GeoMaterial name: " << logVol->getMaterial()->getName() << std::endl;

  // get number of children volumes
  unsigned int nChil = worldPtr->getNChildVols();
  std:: cout << "'world' number of children: " << nChil << std::endl;

  // loop over all children nodes
  std::cout << "Looping over all 'volume' children (i.e., GeoPhysVol and GeoFullPhysVol)..." << std::endl;
  for (unsigned int idx=0; idx<nChil; ++idx) {
	PVConstLink nodeLink = worldPtr->getChildVol(idx);

	if ( dynamic_cast<const GeoVPhysVol*>( &(*( nodeLink ))) ) {
		std::cout << "\t" << "the child n. " << idx << " ";
		const GeoVPhysVol *childVolV = &(*( nodeLink ));
		if ( dynamic_cast<const GeoPhysVol*>(childVolV) )

        {
			const GeoPhysVol* childVol = dynamic_cast<const GeoPhysVol*>(childVolV);
			std::cout << "is a GeoPhysVol, whose GeoLogVol name is: " << childVol->getLogVol()->getName() << std::endl;
            std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";

		} else if ( dynamic_cast<const GeoFullPhysVol*>(childVolV) ) {
			const GeoFullPhysVol* childVol = dynamic_cast<const GeoFullPhysVol*>(childVolV);
			std::cout << "is a GeoFullPhysVol, whose GeoLogVol name is: " << childVol->getLogVol()->getName() << std::endl;
            std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";
		}
        } else if ( dynamic_cast<const GeoNameTag*>( &(*( nodeLink ))) ) {
		std::cout << "\t" << "the child n. " << idx << " is a GeoNameTag" << std::endl;
		const GeoNameTag *childVol = dynamic_cast<const GeoNameTag*>(&(*( nodeLink )));
		std::cout << "\t\tGeoNameTag's name: " << childVol->getName() << std::endl;
        //std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";
        }
  }

  std::cout << "DONE. Geometry tested." << std::endl;

  //------------------------------------------------------------//
  // --- writing the newly created Geometry to a local file --- //
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
  if (db.checkIsDBOpen())
      std::cout << "OK! Database is open!\n";
  else {
      std::cout << "Database ERROR!! Exiting...\n";
      return 1;
  }

   std::cout << "Dumping the GeoModel geometry to the DB file..." << std::endl;
  // Dump the tree volumes into a DB
  GeoModelIO::WriteGeoModel dumpGeoModelGraph(db); // init the GeoModel node action
  world->exec(&dumpGeoModelGraph); // visit all nodes in the GeoModel tree
  dumpGeoModelGraph.saveToDB(); // save to the local SQlite DB file
  std::cout << "DONE. Geometry saved to the file '" << path << "'.\n" <<std::endl;

  return 0; //app.exec();
}
