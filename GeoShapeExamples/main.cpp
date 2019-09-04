/*
 * HelloGeo.cpp
 *
 *  Created on: Apr, 2019
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

// GeoShape nodes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTorus.h"

// Geo core classes
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoTransform.h"

// Geo I/O
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelWrite/WriteGeoModel.h"

// Qt5
#include <QDebug>
#include <QFileInfo>

// C++
#include <iostream>


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'


int main(int argc, char *argv[])
{
	//-----------------------------------------------------------------------------------//
  // create the world volume container and
  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Creating the 'world' volume, i.e. the root volume of the GeoModel tree..." << std::endl;
  double densityOfAir = 0.1;
  const GeoMaterial* worldMat = new GeoMaterial("std::Air", densityOfAir);
  const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, worldMat);
  GeoPhysVol* world = new GeoPhysVol(worldLog);


  //----------------------------------------------------------------------------------//
  // Get the materials
  // const GeoMaterial* matIron = theMaterialManager->getMaterial("std::Iron"); // Athena code
  // Bogus densities.  Later: read from database.
  double densityOfIron= 0.7;
  const GeoMaterial *matIron = new GeoMaterial("Iron Toy",densityOfIron);


	//-----------------------------------------------------------------------------------//
	// Make the boxes that describe the shape of the volumes:                            //
  GeoBox* box = new GeoBox(50*SYSTEM_OF_UNITS::cm, 50*SYSTEM_OF_UNITS::cm, 50*SYSTEM_OF_UNITS::cm);

  GeoTorus* torus = new GeoTorus(10*SYSTEM_OF_UNITS::cm /*Rmin*/,  20*SYSTEM_OF_UNITS::cm /*Rmax*/, 50*SYSTEM_OF_UNITS::cm /*Rtor*/,  0*SYSTEM_OF_UNITS::degree/*SPhi*/,  270*SYSTEM_OF_UNITS::degree/*DPhi*/);

  // GeoBox(17.5*SYSTEM_OF_UNITS::mm, 126.5*SYSTEM_OF_UNITS::mm, 675*SYSTEM_OF_UNITS::mm);
  // GeoBox* part3 = new GeoBox(390*SYSTEM_OF_UNITS::mm, 23*SYSTEM_OF_UNITS::mm, 647.5*SYSTEM_OF_UNITS::mm);
  // GeoBox* part4 = new GeoBox(264*SYSTEM_OF_UNITS::mm, 135*SYSTEM_OF_UNITS::mm, 30*SYSTEM_OF_UNITS::mm);
  // GeoBox* part5 = new GeoBox(177.45*SYSTEM_OF_UNITS::mm, 8*SYSTEM_OF_UNITS::mm, 163*SYSTEM_OF_UNITS::mm);
  // GeoBox* part6 = new GeoBox(300*SYSTEM_OF_UNITS::mm, 250*SYSTEM_OF_UNITS::mm, 25*SYSTEM_OF_UNITS::mm);


  //------------------------------------//
	// Define the space transforms to place the boxes:
  GeoTrf::Translate3D Tr1(-75*SYSTEM_OF_UNITS::cm, 0, 0);
  GeoTrf::Translate3D Tr2(75*SYSTEM_OF_UNITS::cm, 0, 0);

  GeoTransform* tr1 = new GeoTransform(Tr1);
  GeoTransform* tr2 = new GeoTransform(Tr2);

  // GeoTrf::Translate3D Tr3(195.5*SYSTEM_OF_UNITS::mm, 0, 705.05*SYSTEM_OF_UNITS::mm);
  // GeoTrf::Translate3D Tr4(195.5*SYSTEM_OF_UNITS::mm, 118.5*SYSTEM_OF_UNITS::mm, -512*SYSTEM_OF_UNITS::mm);
  // GeoTrf::Translate3D Tr5(195.5*SYSTEM_OF_UNITS::mm, 422.6*SYSTEM_OF_UNITS::mm, -225*SYSTEM_OF_UNITS::mm);


  //------------------------------------//
	// By using GeoShift, place the boxes in the space:
  // GeoShapeShift* sh2 = new GeoShapeShift(part2, Tr1);
  // GeoShapeShift* sh3 = new GeoShapeShift(part3, Tr2);
  // GeoShapeShift* sh4 = new GeoShapeShift(part4, Tr3);
  // GeoShapeShift* sh5 = new GeoShapeShift(part5, Tr4);
  // GeoShapeShift* sh6 = new GeoShapeShift(part6, Tr5);

  //------------------------------------//
	// By using GeoShapeUnion, define a compund object,
	// composed of a union of the previously-defined boxes:
  // GeoShapeUnion* part_Union1 = new GeoShapeUnion(part1, sh2);
  // GeoShapeUnion* part_Union2 = new GeoShapeUnion(part_Union1, sh3);
  // GeoShapeUnion* part_Union3 = new GeoShapeUnion(part_Union2, sh4);
  // GeoShapeUnion* part_Union4 = new GeoShapeUnion(part_Union3, sh5);
  // GeoShapeUnion* part_Union5 = new GeoShapeUnion(part_Union4, sh6);


	//------------------------------------//
	// Bundle the resulting compound object with a material into a logical volume, and create a physical volume with that:
  GeoLogVol* boxLog = new GeoLogVol("Box",box,matIron);
	GeoPhysVol* boxPhys = new GeoPhysVol(boxLog);

  GeoLogVol* torusLog = new GeoLogVol("Torus",torus,matIron);
	GeoPhysVol* torusPhys = new GeoPhysVol(torusLog);



  //------------------------------------//
	// Now insert all of this into the world...                                           //
  world->add(tr1);
	world->add(boxPhys);
  world->add(tr2);
	world->add(torusPhys);



  //------------------------------------//
	// Writing the geometry to file
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
  std::cout << "DONE. Geometry saved." <<std::endl;

  std::cout << "\nTest - list of all the GeoMaterial nodes in the persistified geometry:" << std::endl;
  db.printAllMaterials();


  return 0;
}
