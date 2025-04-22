// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * This example tests shared LogVol nodes.
 *
 * It creates one single LogVol, with one Material and one Shape.
 * Then it creates three PhysVol volumes using the above LogVol.
 *
 * It is meant to how how to share GeoModel nodes to reduce the memory footprint
 * and to test the GeoModel I/O packages and methods.
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Dec, 2022
 *
 */

// local includes
#include "world.h"

// GeoModel includes
#include "GeoModelIOHelpers/GMIO.h"
#include "GeoModelIOHelpers/GMTests_IO.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoTube.h"

// C++ includes
#include <cstdlib>  // EXIT_FAILURE
#include <fstream>
#include <iostream>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS \
    GeoModelKernelUnits  // so we will get, e.g., 'GeoModelKernelUnits::cm'

int main(int argc, char *argv[]) {
// Define the units
#define gr SYSTEM_OF_UNITS::gram
#define mole SYSTEM_OF_UNITS::mole
#define cm3 SYSTEM_OF_UNITS::cm3

    // get the World volume,
    // we build it outside the class for convenience only
    GeoPhysVol *world = GeoModel::World().getPhysVol();

    // Define elements used in this example:
    GeoElement *aluminium =
        new GeoElement("Aluminium", "Al", 13, 26 * gr / mole);
    GeoElement *iron = new GeoElement("Iron", "Fe", 26.0, 55.847 * gr / mole);

    // Define materials:
    double densityOfAluminium = 2.7;  // g/cm^3
    double densityOfIron = 7.87;      // g/cm^3
    GeoMaterial *Aluminium = new GeoMaterial("Aluminium", densityOfAluminium);
    Aluminium->add(aluminium, 1.0);
    Aluminium->lock();

    // define a shape node to share
    const GeoTube *t1Tube = new GeoTube(0.5, 1.0, 2.0);

    // define two LogVol nodes that share the same shape
    const GeoLogVol *t1Log = new GeoLogVol("T1Log", t1Tube, Aluminium);

    // build two PhysVol volumes with the two LogVol nodes
    GeoPhysVol *t1Phys = new GeoPhysVol(t1Log);
    GeoPhysVol *t2Phys = new GeoPhysVol(t1Log);
    GeoPhysVol *t3Phys = new GeoPhysVol(t1Log);

    GeoTransform *xf2 = new GeoTransform(GeoTrf::Translate3D(-10.0, 0, 0));
    GeoTransform *xf3 = new GeoTransform(GeoTrf::Translate3D(10.0, 0, 0));

    // add the first tube to the world
    world->add(t1Phys);

    // add the second tube to the world
    world->add(xf2);
    world->add(t2Phys);

    // add the third tube to the world
    world->add(xf3);
    world->add(t3Phys);

    // --- I/O TESTS ---
    std::cout << "\nTests:\n";
    unsigned loglevel = 0;
    unsigned printtests = 1;
    const bool forceDelete = true;
    std::pair<bool, std::map<std::string, bool>> tests =
        GeoModelIO::TestIO::runAllTests(world, loglevel, printtests, forceDelete);
    bool testall = tests.first;
    if (testall) return 0;
    return 1;
    // ----------------
}
