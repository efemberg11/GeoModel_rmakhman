/*
 * main.cpp
 *
 *  Created on: Sep, 2019
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

// GeoShape nodes
#include "GeoModelKernel/GeoBox.h"
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
        GeoElement*  Sillicon = new GeoElement ("Silicon"  ,"Si" , 14.0 ,  28.085  *gr/mole);

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
	// Create the solids:

	// -- A tessellated solid made of a single 2D quad surface
	GeoFacetVertex quadV1 = GeoFacetVertex(0.,0.,0.);
	GeoFacetVertex quadV2 = GeoFacetVertex(0., 50.*SYSTEM_OF_UNITS::cm, 0.);
	GeoFacetVertex quadV3 = GeoFacetVertex(50.*SYSTEM_OF_UNITS::cm, 50.*SYSTEM_OF_UNITS::cm, 0.);
	GeoFacetVertex quadV4 = GeoFacetVertex(50.*SYSTEM_OF_UNITS::cm,0.,0.);
	GeoQuadrangularFacet* quadFacet = new GeoQuadrangularFacet(quadV1, quadV2, quadV3, quadV4, GeoFacet::ABSOLUTE);
	GeoTessellatedSolid* tessQuad = new GeoTessellatedSolid();
	tessQuad->addFacet(quadFacet);

	// -- A tessellated solid made of a single 3D triangular surface
	GeoFacetVertex triV1 = GeoFacetVertex(0,0,50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex triV2 = GeoFacetVertex(0,50*SYSTEM_OF_UNITS::cm,0);
	GeoFacetVertex triV3 = GeoFacetVertex(50*SYSTEM_OF_UNITS::cm,0,0);
	GeoTriangularFacet* triFacet = new GeoTriangularFacet(triV1, triV2, triV3, GeoFacet::ABSOLUTE);
	GeoTessellatedSolid* tessTri = new GeoTessellatedSolid();
	tessTri->addFacet(triFacet);

	// -- A tessellated solid made of a 3D triangular facet and a 2D quad facet
	GeoTessellatedSolid* tessTriQuad = new GeoTessellatedSolid();
	tessTriQuad->addFacet(triFacet);
	tessTriQuad->addFacet(quadFacet);

	// - A tessellated solid example: a triangle pyramid made out of 4 triangles
	// Render a pyramid consists of 4 triangles
	// Front triangular facet
	GeoFacetVertex v1P = GeoFacetVertex( 0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v2P = GeoFacetVertex(-1.0f, -1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v3P = GeoFacetVertex(1.0f, -1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet1Pyr = new GeoTriangularFacet(v1P, v2P, v3P, GeoFacet::ABSOLUTE);
	// Right triangular facet
	GeoFacetVertex v4P = GeoFacetVertex(0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v5P = GeoFacetVertex(1.0f, -1.0f, 1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v6P = GeoFacetVertex(1.0f, -1.0f, -1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet2Pyr = new GeoTriangularFacet(v4P, v5P, v6P, GeoFacet::ABSOLUTE);
	// Back triangular facet
	GeoFacetVertex v7P = GeoFacetVertex(0.0f, 1.0f, 0.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v8P = GeoFacetVertex(1.0f, -1.0f, -1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoFacetVertex v9P = GeoFacetVertex(-1.0f, -1.0f, -1.0f)*(50*SYSTEM_OF_UNITS::cm);
	GeoTriangularFacet* triFacet3Pyr = new GeoTriangularFacet(v7P, v8P, v9P, GeoFacet::ABSOLUTE);
	// Left triangular facet
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

	// -- A cube can be represented with 8 points, a triangulated_surface_set with pnmax= 24, 24 normals, and a list of 12 triangles.
		// TODO:
//	// Render a color-cube consisting of 6 quads with different colors
//	   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
//	      // Top face (y = 1.0f)
//	      // Define vertices in counter-clockwise (CCW) order with normal pointing out
//	      glColor3f(0.0f, 1.0f, 0.0f);     // Green
//	      glVertex3f( 1.0f, 1.0f, -1.0f);
//	      glVertex3f(-1.0f, 1.0f, -1.0f);
//	      glVertex3f(-1.0f, 1.0f,  1.0f);
//	      glVertex3f( 1.0f, 1.0f,  1.0f);
//
//	      // Bottom face (y = -1.0f)
//	      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
//	      glVertex3f( 1.0f, -1.0f,  1.0f);
//	      glVertex3f(-1.0f, -1.0f,  1.0f);
//	      glVertex3f(-1.0f, -1.0f, -1.0f);
//	      glVertex3f( 1.0f, -1.0f, -1.0f);
//
//	      // Front face  (z = 1.0f)
//	      glColor3f(1.0f, 0.0f, 0.0f);     // Red
//	      glVertex3f( 1.0f,  1.0f, 1.0f);
//	      glVertex3f(-1.0f,  1.0f, 1.0f);
//	      glVertex3f(-1.0f, -1.0f, 1.0f);
//	      glVertex3f( 1.0f, -1.0f, 1.0f);
//
//	      // Back face (z = -1.0f)
//	      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
//	      glVertex3f( 1.0f, -1.0f, -1.0f);
//	      glVertex3f(-1.0f, -1.0f, -1.0f);
//	      glVertex3f(-1.0f,  1.0f, -1.0f);
//	      glVertex3f( 1.0f,  1.0f, -1.0f);
//
//	      // Left face (x = -1.0f)
//	      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
//	      glVertex3f(-1.0f,  1.0f,  1.0f);
//	      glVertex3f(-1.0f,  1.0f, -1.0f);
//	      glVertex3f(-1.0f, -1.0f, -1.0f);
//	      glVertex3f(-1.0f, -1.0f,  1.0f);
//
//	      // Right face (x = 1.0f)
//	      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
//	      glVertex3f(1.0f,  1.0f, -1.0f);
//	      glVertex3f(1.0f,  1.0f,  1.0f);
//	      glVertex3f(1.0f, -1.0f,  1.0f);
//	      glVertex3f(1.0f, -1.0f, -1.0f);
//	   glEnd();  // End of drawing color-cube



	//------------------------------------//
	// Define the space transforms to place the boxes:
	GeoTrf::Translate3D Tr1(-75*SYSTEM_OF_UNITS::cm, 0, 0); // cube
	GeoTrf::Translate3D Tr2( 75*SYSTEM_OF_UNITS::cm, 0, 0); // torus
	GeoTrf::Translate3D Tr3( -75*SYSTEM_OF_UNITS::cm, -150*SYSTEM_OF_UNITS::cm, 0); // tessellated solid quad
	GeoTrf::Translate3D Tr4( 75*SYSTEM_OF_UNITS::cm, -150*SYSTEM_OF_UNITS::cm, 0); // tessellated solid tri
	GeoTrf::Translate3D Tr5( -75*SYSTEM_OF_UNITS::cm, -225*SYSTEM_OF_UNITS::cm, 0); // tessellated solid triquad
	GeoTrf::Translate3D Tr6( 75*SYSTEM_OF_UNITS::cm, -225*SYSTEM_OF_UNITS::cm, 0); // tessellated solid pyramid

	GeoTransform* tr1 = new GeoTransform(Tr1);
	GeoTransform* tr2 = new GeoTransform(Tr2);
	GeoTransform* tr3 = new GeoTransform(Tr3);
	GeoTransform* tr4 = new GeoTransform(Tr4);
	GeoTransform* tr5 = new GeoTransform(Tr5);
	GeoTransform* tr6 = new GeoTransform(Tr6);


	//------------------------------------//
	// Bundle the resulting compound object with a material into a logical volume, and create a physical volume with that:
	GeoLogVol* tess1Log = new GeoLogVol("Tessellated Quad",tessQuad,steel);
	GeoPhysVol* tess1Phys = new GeoPhysVol(tess1Log);

	GeoLogVol* tess2Log = new GeoLogVol("Tessellated Tri",tessTri,steel);
	GeoPhysVol* tess2Phys = new GeoPhysVol(tess2Log);

	GeoLogVol* tess3Log = new GeoLogVol("Tessellated TriQuad",tessTriQuad,steel);
	GeoPhysVol* tess3Phys = new GeoPhysVol(tess3Log);

	GeoLogVol* tess4Log = new GeoLogVol("Tessellated Pyramid",tessPyramid,steel);
	GeoPhysVol* tess4Phys = new GeoPhysVol(tess4Log);



	//------------------------------------//
	// Now insert all of this into the world...                                           //
	world->add(tr1);
	world->add(tess1Phys);
	world->add(tr2);
	world->add(tess2Phys);
	world->add(tr3);
	world->add(tess3Phys);
	world->add(tr4);
	world->add(tess4Phys);
	world->add(tr5);
	world->add(tr6);



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
	std::cout << "\nTest - list of all the GeoShape nodes in the persistified geometry:" << std::endl;
	db.printAllShapes();


	return 0;
}
