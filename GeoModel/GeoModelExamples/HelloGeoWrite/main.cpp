// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * HelloGeoWrite.cpp
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Apr, 2019
 *  Updated on: Nov, 2023 - Riccardo Maria BIANCHI - moved to I/O helpers
 *
 */

// GeoModel includes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelIOHelpers/GMIO.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE
#include <getopt.h> // runtime options


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

/* print a description of all supported options */
void usage (FILE *fp, const char *path)
{
    /* take only the last portion of the path */
    const char *basename = strrchr(path, '/');
    basename = basename ? basename + 1 : path;

    fprintf (fp, "usage: %s [OPTION]\n", basename);
    fprintf (fp, "  -h, --help\t\t"
                 "Print this help and exit.\n");
    fprintf (fp, "  -d, --forceDelete\t"
                 "Force the deletion of an existing output file with the same name (i.e., Replace the existing output file).\n");
}




int main(int argc, char *argv[])
{
    // warning message to teh user
    std::cout << "\n*** NOTE ***\n"
       << "Please, notice: you should use the 'forceDelete' option\n"
       << "if you want to overwrite the existing output file."
       << "\nRun with the '-h' otion to see the flag.\n***\n\n";

    // === Command-Line OPTIONS ===
    int flag_help = 0;
    bool flag_forceDelete = false;
    option longopts[] = {
        { "help", no_argument, &flag_help, 1 },
        {"forceDelete", no_argument, NULL, 'd'}, 
        {0}
    };
    while (1) {
        // declare options 'h' and 'd', both without arguments
        const int opt = getopt_long(argc, argv, "hd", longopts, 0);
        if (opt == -1) {
            /* a return value of -1 indicates that there are no more options */
            break;
        }
        switch (opt) {
            case 'h':
                flag_help = 1;
                break;
            case 'd':
                flag_forceDelete = true;
                break;
            case '?':
                usage (stderr, argv[0]);
                return 1;
            default:
                break;
        }
    }
    if (flag_help) {
        usage (stdout, argv[0]);
        return 0;
    }
  //====================================


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

	// Silicon 100% (Detector)
	GeoMaterial* silicon = new GeoMaterial("Silicon", 2.329 *gr/cm3);
	silicon->add(const_cast<GeoElement*> (Sillicon), 1.0);
	silicon->lock();


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
	const GeoLogVol   *toyLog    = new GeoLogVol("ToyLog", toyBox, silicon);                 //
	// ..And create a physical volume:                                                   //
	GeoPhysVol        *toyPhys   = new GeoPhysVol(toyLog);                               //

	GeoBox       *sPass = new GeoBox(5.0*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm);
	GeoLogVol    *lPass = new GeoLogVol("Passive", sPass, steel);
	GeoPhysVol   *pPass = new GeoPhysVol(lPass);

	GeoBox       *sIPass = new GeoBox(4*SYSTEM_OF_UNITS::cm, 25*SYSTEM_OF_UNITS::cm, 25*SYSTEM_OF_UNITS::cm);
	GeoLogVol    *lIPass = new GeoLogVol("InnerPassive", sIPass, air);
	GeoPhysVol   *pIPass = new GeoPhysVol(lIPass);

	pPass->add(pIPass);
	toyPhys->add(pPass);

	//------------------------------------------------------------------------------------//
	// Now insert all of this into the world...                                           //
	GeoNameTag *tag = new GeoNameTag("Toy");
    world->add(tag);
	world->add(toyPhys);
	//------------------------------------------------------------------------------------//


    //------------------------------------------------------------------------------------//
    // Writing the geometry to file
    //------------------------------------------------------------------------------------//
    std::string path = "geometry.db";

    std::cout << "Dumping the GeoModel geometry to the DB file..." << std::endl;
    GMDBManager db = GeoModelIO::IO::saveToDB(world, path, 0, flag_forceDelete);
    std::cout << "DONE. Geometry saved." <<std::endl;

    std::cout << "\nTest - list of all the GeoMaterial nodes in the persistified geometry:" << std::endl;
    db.printAllMaterials();
    std::cout << "\nTest - list of all the GeoElement nodes in the persistified geometry:" << std::endl;
    db.printAllElements();


  return 0;
}
