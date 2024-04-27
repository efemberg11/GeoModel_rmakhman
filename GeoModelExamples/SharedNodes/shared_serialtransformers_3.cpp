// Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration

/*
 * This example tests shared GeoSerialTransformer nodes.
 *
 * In particular, it tests three serialtransformers that share a single PhysVol
 * volume and:
 * - two of them share the same placing formula
 * - one of them has its own placing formula
 * This test also adds complexity by adding instances of IdentifierTag and SerialIdentifier nodes,
 * and uses the PhysVol used by the SerialTrasnformers by placing it
 * in the world with a Transform and an Indentifier.
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Feb, 2023
 *
 */

// local includes
#include "world.h"

// GeoModel includes
#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/Cos.h"
#include "GeoGenericFunctions/Sin.h"
#include "GeoGenericFunctions/Variable.h"
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelIOHelpers/GMIO.h"
#include "GeoModelIOHelpers/GMTests_IO.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoSerialDenominator.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoTube.h"

// C++ includes
#include <cstdlib>  // EXIT_FAILURE
#include <fstream>
#include <iostream>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS \
    GeoModelKernelUnits  // so we will get, e.g., 'GeoModelKernelUnits::cm'

using namespace GeoGenfun;
using namespace GeoXF;

int main(int argc, char *argv[]) {
// Define the units
#define gr SYSTEM_OF_UNITS::gram
#define mole SYSTEM_OF_UNITS::mole
#define cm3 SYSTEM_OF_UNITS::cm3

    // get the World volume,
    // we build it outside the class for convenience only
    GeoPhysVol *world = GeoModel::World().getPhysVol();

    // Define elements used in this example:
    GeoElement *aluminum = new GeoElement("Aluminum", "Al", 13, 26 * gr / mole);

    // Define materials:
    double densityOfAluminum = 2.7;  // g/cm^3
    GeoMaterial *Aluminum = new GeoMaterial("Aluminum", densityOfAluminum);
    Aluminum->add(aluminum, 1.0);
    Aluminum->lock();

    // define a container box
    const GeoBox *toyBox = new GeoBox(20, 20, 20);
    const GeoLogVol *toyLog = new GeoLogVol("ToyLog", toyBox, Aluminum);  //
    GeoPhysVol *toyPV = new GeoPhysVol(toyLog);

    // define the detector volumes
    const GeoBox *sh1 = new GeoBox(1.0, 1.0, 1.0);
    const GeoLogVol *lv1 = new GeoLogVol("LV1", sh1, Aluminum);
    GeoPhysVol *pv1 = new GeoPhysVol(lv1);  // parent volume

    //--------------------------------------//
    // Now, define two mathematical functions,
    // which are used to parametrically place the volumes
    // in space.
    //--------------------------------------//
    const unsigned int NPLATES = 10;
    Variable i;
    Sin sin;
    GENFUNCTION f = NPLATES * 2 * i;
    GENFUNCTION g = sin(360 * SYSTEM_OF_UNITS::deg / NPLATES * i);
    TRANSFUNCTION t1 = Pow(GeoTrf::TranslateX3D(1.0), f);
    TRANSFUNCTION t2 =
        Pow(GeoTrf::TranslateX3D(1.0), f) * Pow(GeoTrf::TranslateY3D(10.0), g);

    //--------------------------------------//
    // Inside, by the way, the serial transformer
    // will evaluate the functions:
    // HepTransform3D xf = t1(i), for i=1,NPLATES....
    //--------------------------------------//

    GeoSerialDenominator *sd1 = new GeoSerialDenominator("PASSIVE-1-");
    GeoSerialIdentifier *sid1 = new GeoSerialIdentifier(10);
    GeoSerialTransformer *st1 = new GeoSerialTransformer(pv1, &t1, NPLATES);
    toyPV->add(sd1);
    toyPV->add(sid1);
    toyPV->add(st1);

    GeoSerialDenominator *sd2 = new GeoSerialDenominator("PASSIVE-2-");
    GeoSerialIdentifier *sid2 = new GeoSerialIdentifier(20);
    GeoSerialTransformer *st2 = new GeoSerialTransformer(pv1, &t2, NPLATES);
    toyPV->add(sd2);
    toyPV->add(sid2);
    toyPV->add(st2);

    GeoTransform *tr1 = new GeoTransform(GeoTrf::Translate3D(0, -30, 0));
    toyPV->add(tr1);
    GeoSerialDenominator *sd3 = new GeoSerialDenominator("PASSIVE-3-");
    GeoSerialTransformer *st3 = new GeoSerialTransformer(pv1, &t2, NPLATES);
    toyPV->add(sd3);
    toyPV->add(st3);

    GeoTransform *tr2 = new GeoTransform(GeoTrf::Translate3D(0, 30, 0));
    GeoIdentifierTag *id1 = new GeoIdentifierTag(1);
    toyPV->add(tr2);
    toyPV->add(id1);
    toyPV->add(pv1);

    GeoTransform *tr3 = new GeoTransform(GeoTrf::Translate3D(0, 35, 0));
    GeoIdentifierTag *id2 = new GeoIdentifierTag(2);
    toyPV->add(tr3);
    toyPV->add(id2);
    toyPV->add(pv1);

    world->add(toyPV);

    // ===== TESTS ======
    bool alltests = false;

    // --- Manual tests ---
    // Manually declare created objects, to test correctness of persistification
    // of in-memory objects
    std::map<std::string, unsigned long> nmap = GeoModelIO::IO::initNodesMap();
    nmap["Function"] = 2;  // t1, t2  (f and g are persistified with t1 and t2)
    nmap["PhysVol"] = 3;   // world, toy, pv1
    nmap["LogVol"] = 3;    // world, toy, lv1
    nmap["Shape"] = 3;     // world, toy, lv1
    nmap["SerialTransformer"] = 3;  // st1, st2, st3
    nmap["SerialDenominator"] = 3;  // sd1, sd22, sd3
    nmap["Transform"] = 3;          // tr1, tr2, tr3
    nmap["Material"] = 2;           // world::air, pv1::alu
    nmap["Element"] = 5;            // world::air (4), pv1::alu (1)
    nmap["SerialIdentifier"] = 2;   // sid1, sid2
    nmap["IdentifierTag"] = 2;      // id1, id2
    nmap["ChildrenConnections"] =
        16;  // world->toy, toy->[sd1,sid1,st1, sd2,sid2,st2, tr1,sd3,st3,
             // tr2,id1,pv1, tr3,id2,pv1]

    // get number of nodes from in-memory GeoModel through the Write action
    std::map<std::string, unsigned long> memmap =
        GeoModelIO::IO::countTreeMemoryNodesFromVolume(world);
    // print the two maps
    GeoModelIO::IO::printCompareTwoNodesMaps(nmap, memmap);
    // compare the two maps
    if (nmap == memmap) {
        std::cout << "OK! Maps match.\n";
        alltests = true;
    } else {
        std::cout << "ERROR!!! Maps do NOT match!\n";
        alltests = false;
    }
    // --- Automated I/O TESTS ---
    std::cout << "\nAutomated tests:\n";
    unsigned loglevel = 0;
    unsigned printtests = 1;
    const bool forceDelete = true;
    std::pair<bool, std::map<std::string, bool>> tests =
        GeoModelIO::TestIO::runAllTests(world, loglevel, printtests, forceDelete);
    bool testall = tests.first;
    if (testall) {
        alltests = (alltests && testall);
    } else {
        alltests = false;
    }
    // ----------------
    std::cout << "\n\n***\n";
    if (alltests) {
        std::cout << "OK! All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "ERROR!!! Some tests did NOT pass!!!\n";
        return 1;
    }
    return 0;
}
