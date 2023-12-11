// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * this test simply creates a GeoModel .db file
 * through the use of GeoModelDBManager, 
 * then it deletes it.
 *
 * author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 * 2023, Dec 11
 *
 */


// GeoModel includes
#include "GeoModelDBManager/GMDBManager.h"
// c++ includes
#include <fstream>


int main(int argc, char *argv[])
{
    //------------------------------------------------------------------------------------//
    //  Open a geometry file
    //------------------------------------------------------------------------------------//
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
    if (db.checkIsDBOpen()) {
        std::cout << "OK! Database is open!" << std::endl;
    } else {
        std::cout << "Database ERROR!! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Now, we remove the test .db file...\n";
    std::remove(path.c_str());  // delete file
    std::cout << "OK, test .db file removed.\n";


    return 0;
}
