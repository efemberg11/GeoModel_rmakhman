
#ifndef GMTESTS_IO_H
#define GMTESTS_IO_H

#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
// #include "GeoModelWrite/WriteGeoModel.h"
#include "GeoModelIOHelpers/GMIO.h"

namespace GeoModelIO {

class TestIO {
   public:
    TestIO(){};

    /// This function tests the number of nodes in the workflow
    /// Write-Read-Write. In this test, a GeoVPhysVol is taken as a 'world'
    /// volume and the GeoModel tree is persistified into a DB file. Then the
    /// file is loaded and the GeoModel tree restored. Then the in-memory
    /// GeoModel tree is persistified again into a new DB file.
    /// This tests the IO tools that dump the in-memory GeoModel tree to file,
    /// the ones that restore it from the DB file into memory, and the DB I/O
    /// methods.
    /// The number of nodes are taken from the DB, by using the tools offered by
    /// the DB Manager.
    static std::pair<std::string, bool> test_compareWriteReadWriteFromVol(
        const GeoVPhysVol* world, unsigned loglevel = 0) {
        std::string testname = "compareWriteReadWriteFromVol";
        if (loglevel >= 1) {
            std::cout << "\n-- Test: '" << testname << "' -- \n";
        }
        // test DBs
        const std::string db1_name = "geometry_db_test_1.db";
        const std::string db2_name = "geometry_db_test_2.db";

        //-----------------------------------------------//
        // 1 -- Writing the geometry to file - 1st time
        //-----------------------------------------------//
        GMDBManager db1 = GeoModelIO::IO::saveToDB(world, db1_name, loglevel);

        //-----------------------------------------------//
        // 2 -- Get number of nodes from the DB we just saved
        //-----------------------------------------------//
        std::map<std::string, unsigned long> mmap_1 =
            GeoModelIO::IO::countNodesFromDB(db1);

        //-----------------------------------------------//
        // 3 -- Reading back the geometry from file
        //-----------------------------------------------//
        GeoVPhysVol* world2 = GeoModelIO::IO::loadDB(db1_name, loglevel);

        //-----------------------------------------------//
        // 4 -- Writing the geometry to file - 2nd time
        //-----------------------------------------------//
        GMDBManager db2 = GeoModelIO::IO::saveToDB(world2, db2_name, loglevel);

        //-----------------------------------------------//
        // 5 -- Get number of nodes from the DB we created from the restored
        // GeoModel tree
        //-----------------------------------------------//
        std::map<std::string, unsigned long> mmap_2 =
            GeoModelIO::IO::countNodesFromDB(db2);

        //-----------------------------------------------//
        // 6 -- Comparison
        //-----------------------------------------------//

        bool testok = false;
        if (mmap_1 == mmap_2) {
            if (loglevel >= 1) {
                std::cout
                    << "OK! The number of nodes that were persistified is "
                       "equal to the "
                       "number of restored-then-persistified-again "
                       "(Write-Read-Write) nodes.\n";
                std::cout << "----\n";
            }
            testok = true;
        } else {
            if (loglevel >= 1) {
                std::cout << "!!! ERROR !!! The number of nodes that were "
                             "persistified is "
                             "NOT equal to the "
                             "number of restored-then-persistified-again "
                             "(Write-Read-Write) nodes!!!\n";
                if (loglevel >= 2) {
                    GeoModelIO::IO::printCompareTwoNodesMaps(mmap_1, mmap_2);
                }
                std::cout << "----\n";
            }
            testok = false;
        }
        std::pair<std::string, bool> testout{testname, testok};
        return testout;
    }

    /// This functions compares the number of nodes of the in-memory GeoModel
    /// tree to the number of the saved-then-restored nodes. The number of nodes
    /// are taken from the cache of the GeoNodeAction that is used to traverse
    /// the GeoModel tree during the I/O operation, instead of being taken from
    /// the DB.
    static std::pair<std::string, bool> test_compareMemoryVsRestoredFromVol(
        const GeoVPhysVol* world, unsigned loglevel = 0) {
        std::string testname = "compareMemoryVsRestoredFromVol";
        if (loglevel >= 1) {
            std::cout << "\n-- Test: '" << testname << "' -- \n";
            std::cout
                << "1 -- Objects in the starting in-memory 'world' volume..."
                << std::endl;
        }
        // printCountWorldMemoryObjects(world);
        std::map<std::string, unsigned long> mmap_1 =
            GeoModelIO::IO::countTreeMemoryNodesFromVolume(world);
        if (loglevel >= 1) {
            std::cout << "2 -- Saving the in-memory 'world' volume into a .db "
                         "file and "
                         "restoring the GeoModel tree from it..."
                      << std::endl;
        }
        std::string dbname = "test_geometry_db_memory.db";
        GMDBManager db = GeoModelIO::IO::saveToDB(world, dbname, loglevel);
        GeoVPhysVol* world2 = GeoModelIO::IO::loadDB(dbname, loglevel);
        if (loglevel >= 1) {
            std::cout
                << "3 -- Objects in the restored in-memory 'world' volume..."
                << std::endl;
        }
        // printCountWorldMemoryObjects(world2);
        std::map<std::string, unsigned long> mmap_2 =
            GeoModelIO::IO::countTreeMemoryNodesFromVolume(world2);
        // TODO: remove temporary .db files when not needed anymore
        // TODO: add number-by-number comparison as with the other tests!!

        bool testok = false;
        if (mmap_1 == mmap_2) {
            if (loglevel >= 1) {
                std::cout << "OK! The number of starting in-memory nodes is "
                             "equal to the "
                             "number of the restored in-memory nodes.\n";
                std::cout << "----\n";
            }
            testok = true;
        } else {
            if (loglevel >= 1) {
                std::cout
                    << "!!! ERROR !!! The number of teh starting in-memory "
                       "nodes is DIFFERENT from "
                       "the number of the restored in-memory nodes!!!\n";
                if (loglevel >= 2) {
                    std::cout << "Starting in-memory nodes:\n";
                    GeoModelIO::IO::printNodesMap(mmap_1);
                    std::cout << "Restored in-memory nodes:\n";
                    GeoModelIO::IO::printNodesMap(mmap_2);
                }
                std::cout << "----\n";
            }
            testok = false;
        }
        std::pair<std::string, bool> testout{testname, testok};
        return testout;
    }

    /// Utility function to print the number of nodes of a GeoModel tree,
    /// starting from a GeoVPhysVol taken as a 'world' volume.
    static void printCountWorldMemoryObjects(const GeoVPhysVol* world) {
        std::map<std::string, unsigned long> mmap =
            GeoModelIO::IO::countTreeMemoryNodesFromVolume(world);
        for (auto& node : mmap) {
            std::cout << node.first << ": " << node.second << std::endl;
        }
    }

    /// This function accepts an SQLite DB file and compares the number of
    /// GeoModel objects read from the SQLite .db file and the number of objects
    /// subsequently restored in memory. It returns 'true' if the number of all
    /// objects from the DB and in-memory match. It returns 'false' if the
    /// numbers do not match.
    static std::pair<std::string, bool> test_compareLoadedVsRestoredFromDB(
        GMDBManager* db, unsigned loglevel = 0) {
        std::string testname = "compareLoadedVsRestoredFromDB";
        if (loglevel >= 1) {
            std::cout << "\n-- Test: " << testname << " -- \n";
        }
        // setup the GeoModel reader
        GeoModelIO::ReadGeoModel read(db);
        // set loglevel of read action, if > 0
        if (loglevel > 0) {
            read.setLogLevel(loglevel);
        }
        // load the data from the DB
        read.loadDB();

        // count all nodes loaded from the DB by the Read action
        // the number of nodes is taken from the 'Read' GeoNodeAction itself,
        // not the DB.
        std::map<std::string, unsigned long> mmap_loaded =
            GeoModelIO::IO::countLoadedNodesFromReadAction(read);

        // build the GeoModel tree from the loaded nodes
        GeoVPhysVol* rootVolume = read.buildGeoModel();

        // count the number of all the restored nodes;
        // i.e., the nodes of the restored in-memory GeoModel tree
        std::map<std::string, unsigned long> mmap_restored =
            GeoModelIO::IO::countTreeMemoryNodesFromVolume(rootVolume);

        bool testok = false;
        if (mmap_loaded == mmap_restored) {
            if (loglevel >= 1) {
                std::cout << "OK! The number of loaded nodes is equal to the "
                             "number of restored nodes.\n";
            }
            testok = true;
        } else {
            if (loglevel >= 1) {
                std::cout << "!!! ERROR !!! The number of loaded nodes is "
                             "DIFFERENT from "
                             "the number of restored nodes!!!\n";
                if (loglevel >= 2) {
                    std::cout << "Loaded:\n";
                    GeoModelIO::IO::printNodesMap(mmap_loaded);
                    std::cout << "Restored:\n";
                    GeoModelIO::IO::printNodesMap(mmap_restored);
                }
            }
            testok = false;
        }
        std::pair<std::string, bool> testout{testname, testok};
        return testout;
    }

    static std::pair<bool, std::map<std::string, bool>> runAllTests(
        GeoVPhysVol* world, unsigned loglevel = 0, unsigned printtests = 0) {
        std::pair<bool, std::map<std::string, bool>> tests;
        bool testall = false;
        // TEST I/O: loading/restoring
        std::pair<std::string, bool> test1 =
            GeoModelIO::TestIO::test_compareWriteReadWriteFromVol(world,
                                                                  loglevel);
        std::pair<std::string, bool> test2 =
            GeoModelIO::TestIO::test_compareMemoryVsRestoredFromVol(world,
                                                                    loglevel);
        // TEST I/O: DB
        if (loglevel > 0) {
            std::cout
                << "\nSaving the 'world' volume to a .db file, for tests...\n";
        }
        GMDBManager db = GeoModelIO::IO::saveToDB(world, "testdb.db", loglevel);
        std::pair<std::string, bool> test3 =
            GeoModelIO::TestIO::test_compareLoadedVsRestoredFromDB(&db,
                                                                   loglevel);
        // set an overall test return status, if all tests passed
        if (test1.second && test2.second && test3.second) {
            testall = true;
        }
        // gather and return results
        std::map<std::string, bool> tmap;
        tmap[test1.first] = test1.second;
        tmap[test2.first] = test2.second;
        tmap[test3.first] = test3.second;
        tests.first = testall;
        tests.second = tmap;
        if (printtests >= 1) {
            std::cout << "\n---\nThe overall result of the tests is: "
                      << testall << " [" << (testall ? "PASSED." : "FAILED!")
                      << "] \n";
            std::cout << "Single results: \n";
            for (auto& tt : tests.second) {
                std::cout << "test: '" << tt.first << "' ==> " << tt.second
                          << " [" << (testall ? "PASSED." : "FAILED!") << "]"
                          << std::endl;
            }
        }
        return tests;
    }
};  // end class
}  // namespace GeoModelIO

#endif
