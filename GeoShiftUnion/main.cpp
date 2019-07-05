/*
 * HelloGeo.cpp
 *
 *  Created on: Apr, 2019
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoNameTag.h"

#include "GeoModelDBManager/GMDBManager.h"

#include "GeoModelWrite/WriteGeoModel.h"

#include <QDebug>

#include <iostream>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'


int main(int argc, char *argv[])
{
  //-----------------------------------------------------------------------------------//
	// Get the materials that we shall use.                                              //
	// ----------------------------------------------------------------------------------//


	// Get the materials
  // const GeoMaterial* matIron = theMaterialManager->getMaterial("std::Iron"); // Athena code
	// Bogus densities.  Later: read from database.
  double densityOfIron= 0.7;
	double densityOfAir = 0.1;
	const GeoMaterial *air     = new GeoMaterial("Air Toy",densityOfAir);
  const GeoMaterial *matIron = new GeoMaterial("Iron Toy",densityOfIron);


	//-----------------------------------------------------------------------------------//
  // create the world volume container and
  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Creating the 'world' volume, i.e. the root volume of the GeoModel tree..." << std::endl;
  const GeoMaterial* worldMat = new GeoMaterial("std::Air", densityOfAir);
  const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, worldMat);
  GeoPhysVol* world = new GeoPhysVol(worldLog);


	//-----------------------------------------------------------------------------------//
	// Make the boxes that describe the shape of the volumes:                            //
  GeoBox* part1 = new GeoBox(17.5*SYSTEM_OF_UNITS::mm, 126.5*SYSTEM_OF_UNITS::mm, 675*SYSTEM_OF_UNITS::mm);
  GeoBox* part2 = new GeoBox(17.5*SYSTEM_OF_UNITS::mm, 126.5*SYSTEM_OF_UNITS::mm, 675*SYSTEM_OF_UNITS::mm);
  GeoBox* part3 = new GeoBox(390*SYSTEM_OF_UNITS::mm, 23*SYSTEM_OF_UNITS::mm, 647.5*SYSTEM_OF_UNITS::mm);
  GeoBox* part4 = new GeoBox(264*SYSTEM_OF_UNITS::mm, 135*SYSTEM_OF_UNITS::mm, 30*SYSTEM_OF_UNITS::mm);
  GeoBox* part5 = new GeoBox(177.45*SYSTEM_OF_UNITS::mm, 8*SYSTEM_OF_UNITS::mm, 163*SYSTEM_OF_UNITS::mm);
  GeoBox* part6 = new GeoBox(300*SYSTEM_OF_UNITS::mm, 250*SYSTEM_OF_UNITS::mm, 25*SYSTEM_OF_UNITS::mm);


	//-----------------------------------------------------------------------------------//
	// Define the space transforms to place the boxes:                                   //
  GeoTrf::Translate3D Tr1(391*SYSTEM_OF_UNITS::mm, 0, 0*SYSTEM_OF_UNITS::mm);
  GeoTrf::Translate3D Tr2(195.5*SYSTEM_OF_UNITS::mm, 149.55*SYSTEM_OF_UNITS::mm, 27.5*SYSTEM_OF_UNITS::mm);
  GeoTrf::Translate3D Tr3(195.5*SYSTEM_OF_UNITS::mm, 0, 705.05*SYSTEM_OF_UNITS::mm);
  GeoTrf::Translate3D Tr4(195.5*SYSTEM_OF_UNITS::mm, 118.5*SYSTEM_OF_UNITS::mm, -512*SYSTEM_OF_UNITS::mm);
  GeoTrf::Translate3D Tr5(195.5*SYSTEM_OF_UNITS::mm, 422.6*SYSTEM_OF_UNITS::mm, -225*SYSTEM_OF_UNITS::mm);


	//-----------------------------------------------------------------------------------//
	// By using GeoShift, place the boxes in the space:                                  //
  GeoShapeShift* sh2 = new GeoShapeShift(part2, Tr1);
  GeoShapeShift* sh3 = new GeoShapeShift(part3, Tr2);
  GeoShapeShift* sh4 = new GeoShapeShift(part4, Tr3);
  GeoShapeShift* sh5 = new GeoShapeShift(part5, Tr4);
  GeoShapeShift* sh6 = new GeoShapeShift(part6, Tr5);

	//-----------------------------------------------------------------------------------//
	// By using GeoShapeUnion, define a compund object,
	// composed of a union of the previously-defined boxes:                              //
  GeoShapeUnion* part_Union1 = new GeoShapeUnion(part1, sh2);
  GeoShapeUnion* part_Union2 = new GeoShapeUnion(part_Union1, sh3);
  GeoShapeUnion* part_Union3 = new GeoShapeUnion(part_Union2, sh4);
  GeoShapeUnion* part_Union4 = new GeoShapeUnion(part_Union3, sh5);
  GeoShapeUnion* part_Union5 = new GeoShapeUnion(part_Union4, sh6);


	//-----------------------------------------------------------------------------------//
	// Bundle the resulting compound object with a material into a logical volume:       //
  GeoLogVol* partBody_log = new GeoLogVol("Simplified_NJD_Feet_LogVol",part_Union5,matIron);
	// ..And create a physical volume:                                                   //
	GeoPhysVol* partBody_phy = new GeoPhysVol(partBody_log);


	//------------------------------------------------------------------------------------//
	// Now insert all of this into the world...                                           //
	GeoNameTag* tag = new GeoNameTag("Simplified_NJD_Feet");
	world->add(tag);
	world->add(partBody_phy);
	//------------------------------------------------------------------------------------//



	//------------------------------------------------------------------------------------//
	// Writing the geometry to file
	//------------------------------------------------------------------------------------//
	QString path = "geometry.db";
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
  world->exec(&dumpGeoModelGraph); // visit all GeoModel nodes
  dumpGeoModelGraph.saveToDB(); // save to the SQlite DB file
  std::cout << "DONE. Geometry saved." <<std::endl;

  std::cout << "\nTest - list of all the GeoMaterial nodes in the persistified geometry:" << std::endl;
  db.printAllMaterials();


  return 0;
}
