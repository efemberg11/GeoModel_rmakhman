/*
 * HelloGeo.cpp
 *
 *  Created on: Apr, 2019
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoElement.h"

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
	// Get the materials that we shall use.                                              //
	// ----------------------------------------------------------------------------------//

	// Bogus densities.  Later: read from database.
	double densityOfAir=0.1, densityOfAl=2.7;
	
	GeoElement * hydrogen=new GeoElement("Hydrogen"  , "H"    ,1,1.00797);       
	GeoElement * deuterium=new GeoElement("Deuterium" , "D "   ,1,2.01);            
	GeoElement * helium=new GeoElement("Helium"    , "He"   ,2,4.0026);   
	GeoElement * lithium=new GeoElement("Lithium"   , "Li"   ,3,6.9410);  
	GeoElement * beryllium=new GeoElement("Beryllium" , "Be"   ,4,9.0122);  
	GeoElement * boron10=new GeoElement("Boron10"   , "B10"  ,5,10.0);    
	GeoElement * boron11=new GeoElement("Boron11"   , "B11"  ,5,11.0);    
	GeoElement * carbon=new GeoElement("Carbon"    , "C"    ,6,12.011); 
	GeoElement * nitrogen=new GeoElement("Nitrogen"  , "N"    ,7,14.0067); 
	GeoElement * oxygen=new GeoElement("Oxygen"    , "O"    ,8,15.9995 ); 
	GeoElement * fluorine=new GeoElement("Fluorine"  , "F"    ,9,18.9984);
	GeoElement * neon=new GeoElement("Neon"      , "Ne"   ,10,20.179);  
	GeoElement * sodium=new GeoElement("Sodium"    , "Na"   ,11,22.9898);
	GeoElement * magnesium=new GeoElement("Magnesium" , "Mg"   ,12,24.305);  
	GeoElement * aluminium=new GeoElement("Aluminum"  , "Al"   ,13,26.9815); 
	GeoElement * silicon=new GeoElement("Silicon"   , "Si"   ,14,28.086);  
	
	GeoMaterial *air        = new GeoMaterial("Air",densityOfAir);
	air->add(nitrogen,0.79);
	air->add(oxygen,0.21);
	air->lock();
	GeoMaterial *aluminum       = new GeoMaterial("aluminum",densityOfAl);
	aluminum->add(aluminium,1.0);
	aluminum->lock();

  //-----------------------------------------------------------------------------------//
  // create the world volume container and
  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Creating the 'world' volume, i.e. the root volume of the GeoModel tree..." << std::endl;
  
  const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
  GeoPhysVol* world = new GeoPhysVol(worldLog);


	//-----------------------------------------------------------------------------------//
	// Next make the box that describes the shape of the toy volume:                     //
	const GeoBox      *toyBox    = new GeoBox(800*SYSTEM_OF_UNITS::cm, 800*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);                   //
	// Bundle this with a material into a logical volume:                                //
	const GeoLogVol   *toyLog    = new GeoLogVol("ToyLog", toyBox, air);                 //
	// ..And create a physical volume:                                                   //
	GeoPhysVol        *toyPhys   = new GeoPhysVol(toyLog);                               //

	GeoBox       *sPass = new GeoBox(5.0*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm);
	GeoLogVol    *lPass = new GeoLogVol("Passive", sPass, aluminum);
	GeoPhysVol   *pPass = new GeoPhysVol(lPass);

	GeoBox       *sIPass = new GeoBox(4*SYSTEM_OF_UNITS::cm, 25*SYSTEM_OF_UNITS::cm, 25*SYSTEM_OF_UNITS::cm);
	GeoLogVol    *lIPass = new GeoLogVol("InnerPassive", sIPass, air);
	GeoPhysVol   *pIPass = new GeoPhysVol(lIPass);

	pPass->add(pIPass);
	toyPhys->add(pPass);

	//------------------------------------------------------------------------------------//
	// Now insert all of this into the world...                                           //
	GeoNameTag *tag = new GeoNameTag("Toy");                                              //
	world->add(tag);                                                                      //
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
  GMDBManager db(path);

  // check the DB connection
  if (db.isOpen())
      qDebug() << "OK! Database is open!";
  else {
      qDebug() << "Database ERROR!! Exiting...";
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


  return 0;
}
