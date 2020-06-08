/*
 * HelloGeo.cpp
 *
 *  Created on: Nov, 2018
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModel2G4/ExtParameterisedVolumeBuilder.h"
#include "G4LogicalVolume.hh"

// #include "GeoModelUtilities/GeoModelExperiment.h"

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"

#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QFileInfo>

#include <iostream>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'


// GeoModelExperiment* createTheExperiment(GeoPhysVol* world)
// {
//   if (world == nullptr)
//   {
//     // Setup the 'World' volume from which everything else will be suspended
//     double densityOfAir=0.1;
//     const GeoMaterial* worldMat = new GeoMaterial("std::Air", densityOfAir);
//     const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
//     const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, worldMat);
//     world = new GeoPhysVol(worldLog);
//   }
//   // Setup the 'Experiment' manager
//   GeoModelExperiment* theExperiment = new GeoModelExperiment(world);
//   return theExperiment;
// }



int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  // GET GEOMETRY FROM LOCAL DB
  // Set valid db path before first run
  static const QString path = "../geometry.db";
  qDebug() << "Using this DB file:" << path;

  // check if DB file exists. If not, return
  if (! QFileInfo(path).exists() ) {
        qWarning() << "ERROR!! DB '" << path << "' does not exist!!";
        qWarning() << "Returning..." << "\n";
        // return;
        throw;
  }

  // open the DB
  GMDBManager* db = new GMDBManager(path.toStdString());
  /* Open database */
  if (db->checkIsDBOpen()) {
    std::cout << "OK! Database is open!\n";
  }
  else {
    std::cout << "Database is not open!\n";
    // return;
    throw;
  }

  // -- testing the input database
  std::cout << "Printing the list of all GeoMaterial nodes" << std::endl;
  db->printAllMaterials();

  /* setup the GeoModel reader */
  GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);
  qDebug() << "ReadGeoModel set.";


  /* build the GeoModel geometry */
  GeoPhysVol* world = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory and get an handle to the 'world' volume
  qDebug() << "ReadGeoModel::buildGeoModel() done.";



  // --- testing the imported ATLAS Geometry

  // get the GeoLogVol used for the 'world' volume
  std::cout << "Getting the GeoLogVol used by the 'world' volume" << std::endl;
  const GeoLogVol* logVol = world->getLogVol();
  std::cout << "'world' GeoLogVol name: " << logVol->getName() << std::endl;
  std::cout << "'world' GeoMaterial name: " << logVol->getMaterial()->getName() << std::endl;

  // get number of children volumes
  unsigned int nChil = world->getNChildVols();
  std:: cout << "'world' number of children: " << nChil << std::endl;

  // loop over all children nodes
  std::cout << "Looping over all 'volume' children (i.e., GeoPhysVol and GeoFullPhysVol)..." << std::endl;
  for (unsigned int idx=0; idx<nChil; ++idx) {
	PVConstLink nodeLink = world->getChildVol(idx);

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
		qDebug() << "\t" << "the child n. " << idx << " is a GeoNameTag";
		const GeoNameTag *childVol = dynamic_cast<const GeoNameTag*>(&(*( nodeLink )));
		std::cout << "\t\tGeoNameTag's name: " << childVol->getName() << std::endl;
        //std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";
        }
  }

  // build the Geant4 geometry and get an hanlde to the world' volume
  ExtParameterisedVolumeBuilder* builder = new ExtParameterisedVolumeBuilder("ATLAS");
  std::cout << "Building G4 geometry."<<std::endl;
  G4LogicalVolume* g4World = builder->Build(world);

  qDebug() << "Everything done.";

  return app.exec();
}
