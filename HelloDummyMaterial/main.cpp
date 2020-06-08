/*
 * HelloDummyMaterial.cpp
 *
 *  Created on: Apr, 2019
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"

#include "GeoModelDBManager/GMDBManager.h"

#include "GeoModelWrite/WriteGeoModel.h"

#include <QDebug>
#include <QFileInfo>

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
	GeoElement*  dummyElement = new GeoElement ("DummyElement" ,"DD",  99.0 ,  1. *gr/mole);
  GeoElement*  Sillicon = new GeoElement ("Silicon"  ,"Si" , 14.0 ,  28.085  *gr/mole);

	// Define the materials
  // a "Dummy" material
	GeoMaterial *dummyMaterial = new GeoMaterial("GeoModelDummyMaterial", 1.*gr/cm3);
	dummyMaterial->add(dummyElement, 1.0);
	dummyMaterial->lock();
  // Silicon 100% (Detector)
	GeoMaterial* silicon = new GeoMaterial("Silicon", 2.329 *gr/cm3);
	silicon->add(const_cast<GeoElement*> (Sillicon), 1.0);
	silicon->lock();

  //-----------------------------------------------------------------------------------//
  // create the world volume container and
  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Creating the 'world' volume, i.e. the root volume of the GeoModel tree..." << std::endl;
  const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, silicon);
  GeoPhysVol* world = new GeoPhysVol(worldLog);

	//-----------------------------------------------------------------------------------//
	// Make the box that describes the shape of the toy volume:                     //
	const GeoBox      *toyBox         = new GeoBox(800*SYSTEM_OF_UNITS::cm, 800*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);                   //
	const GeoBox      *toyBoxChild    = new GeoBox(400*SYSTEM_OF_UNITS::cm, 400*SYSTEM_OF_UNITS::cm, 500*SYSTEM_OF_UNITS::cm);                   //
	// Bundle this with a material into a logical volume:                                //
	const GeoLogVol   *toyLog         = new GeoLogVol("ToyLog", toyBox, silicon);                 //
	const GeoLogVol   *toyLogChild    = new GeoLogVol("ToyLogUnderConstruction", toyBoxChild, dummyMaterial);                 //
	// ..And create a physical volume:                                                   //
	GeoPhysVol        *toyPhys        = new GeoPhysVol(toyLog);                               //
  GeoPhysVol        *toyPhysChild   = new GeoPhysVol(toyLogChild);                               //

  // add the child node under the parent node
  toyPhys->add(toyPhysChild);

  //------------------------------------------------------------------------------------//
	// Now insert all of this into the world...                                           //
	// GeoNameTag *tag = new GeoNameTag("Toy");                                              //
	// world->add(tag);                                                                      //
	world->add(toyPhys);                                                                  //

	//------------------------------------------------------------------------------------//

	//------------------------------------------------------------------------------------//
	// Writing the geometry to file
	//------------------------------------------------------------------------------------//
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
  GMDBManager db(path.toStdString());

  // check the DB connection
  if (db.checkIsDBOpen())
      std::cout << "OK! Database is open!\n";
  else {
      std::cout << "Database ERROR!! Exiting...\n";
      return 1;
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

  db.printAllLogVols();
  db.printAllPhysVols();
  db.printAllChildrenPositions();

  return 0;
}
