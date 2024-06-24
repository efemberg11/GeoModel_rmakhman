
// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * This header file provides helper functions to write and read a GeoModel SQLite file. 
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Feb, 2023
 *
 */


#ifndef GMIO_H
#define GMIO_H

// C++ includes
#include <cstdlib>  // EXIT_FAILURE
#include <fstream>
#include <string>

#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModelWrite/WriteGeoModel.h"

#include "GeoModelHelpers/throwExcept.h"

namespace GeoModelIO {

class IO {
   public:

    // dummy constructor
    IO(){};

    static GMDBManager saveToDB(const GeoVPhysVol* world, const std::string path,
                                unsigned loglevel = 0, const bool forceDelete = false ) {
        // Check if the output DB file exists already. 
        // - If yes and the 'forceDelete' option is set to 'true' by the user, 
        // then delete it before trying to create the new one; that is, 
        // the 'forceDelete' option replaces the existing '.db' file.
        // - If yes and the 'forceDelete' option is set to 'false', then throw
        // and error and exits. 
        std::ifstream inputfile(path.c_str());
        if (inputfile.good()) {
            if (true == forceDelete) {
                std::cout << "GeoModelIO -- INFO: you set the 'forceDelete' option to 'true', so we replace the existing .db file: '" << path << "'." << std::endl;
                std::remove(path.c_str());  // delete file
            } else {
            std::cerr << "\n*** ERROR! The output '" << path << "' file exists already! If you want to replace it, set the 'forceDelete' option to 'true'. Otherwise, rename the existing file, or move it to another place, and retry. ***\n\n{"
                  << __func__ << " ["
                  << __PRETTY_FUNCTION__ 
                  << "]}\n\n";
                throw;
            }
        }
        inputfile.close();

        // open the DB connection
        GMDBManager db(path);

        // check the DB connection
        if (!db.checkIsDBOpen()) {
            std::cout << "Database ERROR!! Exiting..." << std::endl;
            THROW_EXCEPTION("It was not possible to open the DB correctly!");
        }

        // Dump the tree volumes to a local file
        std::cout << "Dumping the GeoModel geometry to the DB file..."
                  << std::endl;
        // init the GeoModel node action
        GeoModelIO::WriteGeoModel dumpGeoModelGraph(db);
        // set loglevel of write action, if > 0
        if (loglevel > 0) {
            dumpGeoModelGraph.setLogLevel(loglevel);
        }
        world->exec(&dumpGeoModelGraph);  // visit all GeoModel nodes
        dumpGeoModelGraph.saveToDB();     // save to the SQlite DB file
        std::cout << "DONE. Geometry saved." << std::endl;
        return db;
    }

    static const GeoVPhysVol* loadDB(const std::string path, unsigned loglevel = 0) {
        // check if DB file exists. 
        // If not, print a warning message and return a nullptr.
        std::ifstream inputfile(path.c_str());
        bool failed = false;
        if (!inputfile.good()) {
            std::cerr << "\n*** WARNING! The input .db file does not exist! Check the path of the input file. Returning a nullptr... ***\n{"
                  << __func__ << " ["
                  << __PRETTY_FUNCTION__ 
                  << "]}\n\n";
            failed = true;
        }
        inputfile.close();
        if(failed) {
            return nullptr;
        }

        // open the DB
        GMDBManager* db = new GMDBManager(path);
        if (!db->checkIsDBOpen()) {
            std::cout << "ERROR!! -- Database is not open!\n";
            THROW_EXCEPTION("It was not possible to open the DB correctly!");
        }

        /* setup the GeoModel reader */
        GeoModelIO::ReadGeoModel geoReader = GeoModelIO::ReadGeoModel(db);
        // set loglevel of read action, if > 0
        if (loglevel > 0) {
            geoReader.setLogLevel(loglevel);
        }

        /* build the GeoModel geometry */
        // builds the whole GeoModel tree in memory
        const GeoVPhysVol* rootVolume = geoReader.buildGeoModel();

        delete db;
        db = nullptr;

        return rootVolume;  // FIXME: See if you can pass a smart ptr
    }

    static GeoModelIO::ReadGeoModel getReaderDB(const std::string path, unsigned loglevel = 0) {
        // check if DB file exists. 
        // If not, print a warning message and exit.
        std::ifstream inputfile(path.c_str());
        if (!inputfile.good()) {
            std::cerr << "\n*** ERROR! The input .db file does not exist! Check the path of the input file. Exiting...\n"
                  << "{ " << __func__ << " "
                  << "[" << __PRETTY_FUNCTION__ 
                  << "]}.\n\n";
            exit(EXIT_FAILURE);
        }
        inputfile.close();

        // open the DB
        GMDBManager* db = new GMDBManager(path);
        if (!db->checkIsDBOpen()) {
            std::cout << "ERROR!! -- Database is not open!\n";
            THROW_EXCEPTION("It was not possible to open the DB correctly!");
        }

        /* setup the GeoModel reader */
        GeoModelIO::ReadGeoModel geoReader = GeoModelIO::ReadGeoModel(db);
        // set loglevel of read action, if > 0
        if (loglevel > 0) {
            geoReader.setLogLevel(loglevel);
        }

        return geoReader;  // FIXME: See if you can pass a smart ptr
    }

    static std::map<std::string, unsigned long> countNodesFromDB(
        GMDBManager& db) {
        // map to populate and return
        std::map<std::string, unsigned long> mmap;
        // get the number of all nodes in the DB from the DB manager
        unsigned long nphysvols = db.getTableFromNodeType_String("GeoPhysVol").size();
        unsigned long nfullphysvols = db.getTableFromNodeType_String("GeoFullPhysVol").size();
        unsigned long nlogvols = db.getTableFromNodeType_VecVecData("GeoLogVol").size();
        unsigned long nelements = db.getTableFromNodeType_VecVecData("GeoElement").size();
        unsigned long nmaterials = db.getTableFromNodeType_VecVecData("GeoMaterial").size();
        unsigned long nalignables = db.getTableFromNodeType_String("GeoAlignableTransform").size();
        unsigned long nfunctions = db.getTableFromNodeType_VecVecData("Function").size();
        unsigned long nserialtransformers = db.getTableFromNodeType_String("GeoSerialTransformer").size();
        unsigned long nshapes = db.getTableFromNodeType_String("GeoShape").size();
        unsigned long nserialdenominators = db.getTableFromNodeType_String("GeoSerialDenominator").size();
        // get metadata
        unsigned long nchildrenconnections = db.getChildrenTable().size();

        return mmap;
    }

    static std::map<std::string, unsigned long> countTreeMemoryNodesFromVolume(
        const GeoVPhysVol* world, unsigned loglevel = 0) {
        std::map<std::string, unsigned long> mmap;

        // init the graph action to count all nodes in the in-memory tree
        GeoModelIO::WriteGeoModel dump;
        // set loglevel of write action, if > 0
        if (loglevel > 0) {
            dump.setLogLevel(loglevel);
        }
        world->exec(&dump);  // visit all GeoModel nodes

        unsigned long nphysvols = dump.getNPhysVols();
        unsigned long nfullphysvols = dump.getNFullPhysVols();
        unsigned long nlogvols = dump.getNLogVols();
        unsigned long nelements = dump.getNElements();
        unsigned long nmaterials = dump.getNMaterials();
        unsigned long nalignables = dump.getNAlignableTransforms();
        unsigned long nfunctions = dump.getNFunctions();
        unsigned long nserialtransformers = dump.getNSerialTransformers();
        unsigned long nshapes = dump.getNShapes();
        unsigned long nserialdenominators = dump.getNSerialDenominators();
        unsigned long nchildrenconnections = dump.getNChildrenConnections();
        // unsigned nrootvolume = dump.getNRootVolume();
        unsigned long ntransforms = dump.getNTransforms();
        unsigned long nserialidentifiers = dump.getNSerialIdentifiers();
        unsigned long nidentifiertags = dump.getNIdentifierTags();
        unsigned long nnametags = dump.getNNameTags();

        mmap["PhysVol"] = nphysvols;
        mmap["FullPhysVol"] = nfullphysvols;
        mmap["LogVol"] = nlogvols;
        mmap["Element"] = nelements;
        mmap["Material"] = nmaterials;
        mmap["Alignable"] = nalignables;
        mmap["Function"] = nfunctions;
        mmap["SerialTransformer"] = nserialtransformers;
        mmap["Shape"] = nshapes;
        mmap["SerialDenominator"] = nserialdenominators;
        mmap["ChildrenConnections"] = nchildrenconnections;
        mmap["Transform"] = ntransforms;
        mmap["SerialIdentifier"] = nserialidentifiers;
        mmap["IdentifierTag"] = nidentifiertags;
        mmap["NameTag"] = nnametags;

        return mmap;
    }
    static std::map<std::string, unsigned long> countLoadedNodesFromReadAction(
        GeoModelIO::ReadGeoModel& read) {
        std::map<std::string, unsigned long> mmap;

        unsigned long nphysvols = read.getNPhysVols();
        unsigned long nfullphysvols = read.getNFullPhysVols();
        unsigned long nlogvols = read.getNLogVols();
        unsigned long nelements = read.getNElements();
        unsigned long nmaterials = read.getNMaterials();
        unsigned long nalignables = read.getNAlignableTransforms();
        unsigned long nfunctions = read.getNFunctions();
        unsigned long nserialtransformers = read.getNSerialTransformers();
        unsigned long nshapes = read.getNShapes();
        unsigned long nserialdenominators = read.getNSerialDenominators();
        unsigned long nchildrenconnections = read.getNChildrenConnections();
        // unsigned nrootvolume = read.getNRootVolume();
        unsigned long ntransforms = read.getNTransforms();
        unsigned long nserialidentifiers = read.getNSerialIdentifiers();
        unsigned long nidentifiertags = read.getNIdentifierTags();
        unsigned long nnametags = read.getNNameTags();

        mmap["PhysVol"] = nphysvols;
        mmap["FullPhysVol"] = nfullphysvols;
        mmap["LogVol"] = nlogvols;
        mmap["Element"] = nelements;
        mmap["Material"] = nmaterials;
        mmap["Alignable"] = nalignables;
        mmap["Function"] = nfunctions;
        mmap["SerialTransformer"] = nserialtransformers;
        mmap["Shape"] = nshapes;
        mmap["SerialDenominator"] = nserialdenominators;
        mmap["ChildrenConnections"] = nchildrenconnections;
        mmap["Transform"] = ntransforms;
        mmap["SerialIdentifier"] = nserialidentifiers;
        mmap["IdentifierTag"] = nidentifiertags;
        mmap["NameTag"] = nnametags;

        return mmap;
    }

    static void printNodesMap(std::map<std::string, unsigned long> mmap) {
        for (auto& node : mmap) {
            std::cout << node.first << ": " << node.second << std::endl;
        }
    }

    static void printKeyMaps(std::string key,
                             std::map<std::string, unsigned long> m1,
                             std::map<std::string, unsigned long> m2) {
        std::cout << "number of " << key << " : " << m1[key] << " -- "
                  << m2[key] << std::endl;
    }

    static std::vector<std::string> getMapKeys() {
        std::vector<std::string> keys{"PhysVol",
                                      "FullPhysVol",
                                      "LogVol",
                                      "Element",
                                      "Material",
                                      "Alignable",
                                      "Function",
                                      "SerialTransformer",
                                      "Shape",
                                      "Transform",
                                      "SerialIdentifier",
                                      "IdentifierTag",
                                      "NameTag"
                                      "SerialDenominator",
                                      "ChildrenConnections"};
        return keys;
    }
    static void printCompareTwoNodesMaps(
        std::map<std::string, unsigned long> m1,
        std::map<std::string, unsigned long> m2) {
        std::vector<std::string> keys = getMapKeys();
        for (auto& key : keys) {
            printKeyMaps(key, m1, m2);
        }
    }

    static std::map<std::string, unsigned long> initNodesMap() {
        std::map<std::string, unsigned long> nmap;
        std::vector<std::string> keys = getMapKeys();
        // init the map
        for (auto& key : keys) {
            nmap[key] = 0;
        }
        return nmap;
    }
};

}  // namespace GeoModelIO

#endif
