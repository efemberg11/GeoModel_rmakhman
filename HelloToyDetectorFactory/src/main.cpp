/*
 * HelloGeo.cpp
 *
 *  Created on: Nov, 2018
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */


#include "GeoModelExperiment.h"
#include "ToyDetectorFactory.h"

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"

#include "GeoModelDBManager/GMDBManager.h"

#include "GeoModelWrite/WriteGeoModel.h"

#include <QDebug>
#include <QString>
#include <QFileInfo>

#include <iostream>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'


GeoModelExperiment* createTheExperiment(GeoPhysVol* world = nullptr)
{
  if (world == nullptr)
  {
    // Setup the 'World' volume from which everything else will be suspended
    double densityOfAir=0.1;
    const GeoMaterial* worldMat = new GeoMaterial("std::Air", densityOfAir);
    const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
    const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, worldMat);
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
  QString path = "geometry.db";

  // check if DB file exists. If not, return.
  // TODO: this check should go in the 'GMDBManager' constructor.
  if ( QFileInfo(path).exists() ) {
        qWarning() << "\n\tERROR!! A '" << path << "' file exists already!! Please, remove it before running this program.";
        qWarning() << "\tReturning..." << "\n";
        // return;
        exit(1);
  }

	// open the DB connection
  GMDBManager db(path);

  // check the DB connection
  if (db.isOpen())
      qDebug() << "OK! Database is open!";
  else {
      qDebug() << "Database ERROR!! Exiting...";
      return 1;
  }

   std::cout << "Dumping the GeoModel geometry to the DB file..." << std::endl;
  // Dump the tree volumes into a DB
  GeoModelIO::WriteGeoModel dumpGeoModelGraph(db); // init the GeoModel node action
  world->exec(&dumpGeoModelGraph); // visit all nodes in the GeoModel tree
  dumpGeoModelGraph.saveToDB(); // save to the local SQlite DB file
  std::cout << "DONE. Geometry saved to the file '" << path.toStdString() << "'.\n" <<std::endl;



  return 0; //app.exec();
}
