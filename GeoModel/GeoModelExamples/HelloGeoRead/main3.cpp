// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * HelloGeoRead_3
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Nov, 2018
 *  Updated on: Oct, 2023
 *
 */

// GeoModel includes
#include "GeoModelDBManager/GMDBManager.h"

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelRead/ReadGeoModel.h"

// C++ includes
#include <cstdlib>  // EXIT_FAILURE
#include <fstream>
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "\nERROR!\nUsage: %s <geometry.db>\n\n", argv[0]);
        return 1;
    }

    // Get from command-line the input SQLite '.db' file containing the geometry
    std::string line;
    std::string fileName;
    fileName = argv[1];
    std::cout << "Using this SQLite '.db' file:" << fileName << std::endl;


    // check if DB file exists. If not, return.
    std::ifstream inputfile(fileName.c_str());
    if (!inputfile.good()) {
        std::cout << "\n\tERROR!! A '" << fileName
                  << "' file does not exist!! Please, check the path of the "
                     "input file before running this program. Exiting...";
        exit(EXIT_FAILURE);
    }
    inputfile.close();

    // open the DB
    GMDBManager* db = new GMDBManager(fileName);
    /* Open database */
    if (db->checkIsDBOpen()) {
        std::cout << "OK! Database is open!\n";
    } else {
        std::cout << "Database is not open!\n";
        // return;
        throw;
    }

    /* testing the input database */
    std::cout << "Reading records from the database..." << std::endl;
    std::cout << "Printing the list of all GeoMaterial nodes" << std::endl;
    db->printAllMaterials();
    std::cout << "Printing the list of all GeoElement nodes" << std::endl;
    db->printAllElements();

    /* set the GeoModel reader */
    GeoModelIO::ReadGeoModel geoReader = GeoModelIO::ReadGeoModel(db);
    std::cout << "OK! ReadGeoModel is set." << std::endl;
    
    /* reading data from the imported .db file by usinhg the reader object */
    std::cout << "Reading the same records from the imported geometry DB file..."
              << std::endl;
    geoReader.printDBTable("Materials");
    geoReader.printDBTable("Elements");


    /* build the GeoModel tree and load it in memory */
    const GeoVPhysVol* world = geoReader.buildGeoModel(); 


    // --- Reading the properties of the 'world' volume retrieved from the .db file
    std::cout << "Getting the GeoLogVol used by the 'world' volume"
              << std::endl;
    const GeoLogVol* logVol = world->getLogVol();
    std::cout << "'world' GeoLogVol name: " << logVol->getName() << std::endl;
    std::cout << "'world' GeoMaterial name: "
              << logVol->getMaterial()->getName() << std::endl;

    // --- Reading the imported Geometry

    // get number of children volumes
    unsigned int nChil = world->getNChildVols();
    std::cout << "'world' number of children: " << nChil << std::endl;

    // loop over all children nodes
    std::cout << "Looping over all 'volume' children (i.e., GeoPhysVol and "
                 "GeoFullPhysVol)..."
              << std::endl;
    for (unsigned int idx = 0; idx < nChil; ++idx) {
        PVConstLink nodeLink = world->getChildVol(idx);

        if (dynamic_cast<const GeoVPhysVol*>(&(*(nodeLink)))) {
            std::cout << "\t"
                      << "the child n. " << idx << " ";
            const GeoVPhysVol* childVolV = &(*(nodeLink));

            if (dynamic_cast<const GeoPhysVol*>(childVolV)) {
                const GeoPhysVol* childVol =
                    dynamic_cast<const GeoPhysVol*>(childVolV);
                std::cout << "is a GeoPhysVol, whose GeoLogVol name is: "
                          << childVol->getLogVol()->getName();
                std::cout << " and it has  " << childVol->getNChildVols()
                          << " child volumes" << std::endl;
            } else if (dynamic_cast<const GeoFullPhysVol*>(childVolV)) {
                const GeoFullPhysVol* childVol =
                    dynamic_cast<const GeoFullPhysVol*>(childVolV);
                std::cout << "is a GeoFullPhysVol, whose GeoLogVol name is: "
                          << childVol->getLogVol()->getName();
                std::cout << " and it has  " << childVol->getNChildVols()
                          << " child volumes" << std::endl;
            }
        } else if (dynamic_cast<const GeoNameTag*>(&(*(nodeLink)))) {
            std::cout << "\t"
                      << "the child n. " << idx << " is a GeoNameTag"
                      << std::endl;
            const GeoNameTag* childVol =
                dynamic_cast<const GeoNameTag*>(&(*(nodeLink)));
            std::cout << "\t\t GeoNameTag's name: " << childVol->getName()
                      << std::endl;
        } else if (dynamic_cast<const GeoMaterial*>(&(*(nodeLink)))) {
            std::cout << "\t"
                      << "the child n. " << idx << " is a GeoMaterial"
                      << std::endl;
            const GeoMaterial* childVol =
                dynamic_cast<const GeoMaterial*>(&(*(nodeLink)));
            std::cout << "\t\t GeoMaterial's name: " << childVol->getName()
                      << std::endl;
            std::cout << "\t\t GeoMaterial's number of elements: "
                      << childVol->getNumElements() << std::endl;
        }
    }

    std::cout << "Everything done." << std::endl;

    // return app.exec();
    return 0;
}
