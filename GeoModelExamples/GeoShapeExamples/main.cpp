// Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

/*
 * main.cpp
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Aug, 2019
 *
 */

// GeoShape nodes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoTessellatedSolid.h"

// Geo core classes
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoTransform.h"

// Geo I/O
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelWrite/WriteGeoModel.h"

// C++
#include <iostream>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE


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
	// Create the shapes:                            //

	// A box
	GeoBox* box = new GeoBox(50.*SYSTEM_OF_UNITS::cm, 50.*SYSTEM_OF_UNITS::cm, 50.*SYSTEM_OF_UNITS::cm);

	// A cone
	GeoCons* cons = new GeoCons(10.*SYSTEM_OF_UNITS::cm, 20.*SYSTEM_OF_UNITS::cm, 30.*SYSTEM_OF_UNITS::cm, 40.*SYSTEM_OF_UNITS::cm, 25.*SYSTEM_OF_UNITS::cm, 0.*SYSTEM_OF_UNITS::degree, 270.*SYSTEM_OF_UNITS::degree);

	// A torus
	GeoTorus* torus = new GeoTorus(10*SYSTEM_OF_UNITS::cm /*Rmin*/,  20*SYSTEM_OF_UNITS::cm /*Rmax*/, 50*SYSTEM_OF_UNITS::cm /*Rtor*/,  0*SYSTEM_OF_UNITS::degree/*SPhi*/,  270*SYSTEM_OF_UNITS::degree/*DPhi*/);

	// A Tessellated Solid :
	// a triangle pyramid made out of 4 triangles
	// Front facet
	GeoFacetVertex v1P = GeoFacetVertex( 0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v2P = GeoFacetVertex(-1.0f, -1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v3P = GeoFacetVertex(1.0f, -1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet1Pyr = new GeoTriangularFacet(v1P, v2P, v3P, GeoFacet::ABSOLUTE);
	// Right facet
	GeoFacetVertex v4P = GeoFacetVertex(0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v5P = GeoFacetVertex(1.0f, -1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v6P = GeoFacetVertex(1.0f, -1.0f, -1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet2Pyr = new GeoTriangularFacet(v4P, v5P, v6P, GeoFacet::ABSOLUTE);
	// Back facet
	GeoFacetVertex v7P = GeoFacetVertex(0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v8P = GeoFacetVertex(1.0f, -1.0f, -1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v9P = GeoFacetVertex(-1.0f, -1.0f, -1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet3Pyr = new GeoTriangularFacet(v7P, v8P, v9P, GeoFacet::ABSOLUTE);
	// Left facet
	GeoFacetVertex v10P = GeoFacetVertex( 0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v11P = GeoFacetVertex(-1.0f,-1.0f,-1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v12P = GeoFacetVertex(-1.0f,-1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet4Pyr = new GeoTriangularFacet(v10P, v11P, v12P, GeoFacet::ABSOLUTE);
	// build the tessellated solid
	GeoTessellatedSolid* tessPyramid = new GeoTessellatedSolid();
	tessPyramid->addFacet(triFacet1Pyr);
	tessPyramid->addFacet(triFacet2Pyr);
	tessPyramid->addFacet(triFacet3Pyr);
	tessPyramid->addFacet(triFacet4Pyr);



	//------------------------------------//
	// Define the space transforms to place the boxes:
	GeoTrf::Translate3D Tr1(-75*SYSTEM_OF_UNITS::cm, 0, 0); // cube
	GeoTrf::Translate3D Tr2( 75*SYSTEM_OF_UNITS::cm, 0, 0); // torus
	GeoTrf::Translate3D Tr3( -75*SYSTEM_OF_UNITS::cm, -150*SYSTEM_OF_UNITS::cm, 0); // tessellated solid quad
	GeoTrf::Translate3D Tr4( 75*SYSTEM_OF_UNITS::cm, -150*SYSTEM_OF_UNITS::cm, 0); // tessellated solid tri
//	GeoTrf::Translate3D Tr5( -75*SYSTEM_OF_UNITS::cm, -225*SYSTEM_OF_UNITS::cm, 0); // tessellated solid triquad
//	GeoTrf::Translate3D Tr6( 75*SYSTEM_OF_UNITS::cm, -225*SYSTEM_OF_UNITS::cm, 0); // tessellated solid pyramid

	GeoTransform* tr1 = new GeoTransform(Tr1);
	GeoTransform* tr2 = new GeoTransform(Tr2);
	GeoTransform* tr3 = new GeoTransform(Tr3);
	GeoTransform* tr4 = new GeoTransform(Tr4);
//	GeoTransform* tr5 = new GeoTransform(Tr5);
//	GeoTransform* tr6 = new GeoTransform(Tr6);


	//------------------------------------//
	// Bundle the resulting compound object with a material into a logical volume, and create a physical volume with that:
	GeoLogVol* boxLog = new GeoLogVol("Box",box,matIron);
	GeoPhysVol* boxPhys = new GeoPhysVol(boxLog);

	GeoLogVol* torusLog = new GeoLogVol("Torus",torus,matIron);
	GeoPhysVol* torusPhys = new GeoPhysVol(torusLog);

	GeoLogVol* consLog = new GeoLogVol("Cons",cons,matIron);
	GeoPhysVol* consPhys = new GeoPhysVol(consLog);

	GeoLogVol* tessLog = new GeoLogVol("Tessellated Pyramid",tessPyramid,matIron);
	GeoPhysVol* tessPhys = new GeoPhysVol(tessLog);



	//------------------------------------//
	// Now insert all of this into the world...                                           //
	world->add(tr1);
	world->add(boxPhys);
	world->add(tr2);
	world->add(torusPhys);
	world->add(tr3);
	world->add(consPhys);
	world->add(tr4);
	world->add(tessPhys);


	//------------------------------------//
	// Writing the geometry to file
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
	std::cout << "DONE. Geometry saved." <<std::endl;

	std::cout << "\nTest - list of all the GeoMaterial nodes in the persistified geometry:" << std::endl;
	db.printAllMaterials();
	std::cout << "\nTest - list of all the GeoShape nodes in the persistified geometry:" << std::endl;
	db.printAllShapes();


	return 0;
}
