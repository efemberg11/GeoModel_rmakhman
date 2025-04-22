// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * This example tests shared PhysVol nodes.
 *
 * It creates one single LogVol, with one Material and one Shape.
 * Then it creates three PhysVol volumes using the above LogVol.
 * Then, one of those PhysVols is used to create children of the above volumes.
 *
 * It is meant to how how to share GeoModel nodes to reduce the memory footprint
 * and to test the GeoModel I/O packages and methods.
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Feb, 2023
 *
 */

// local includes
#include "world.h"

// GeoModel includes
#include "GeoModelIOHelpers/GMIO.h"
#include "GeoModelIOHelpers/GMTests_IO.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"

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
    GeoElement *aluminum = new GeoElement("Aluminum", "Al", 13, 26 * gr / mole);

    // Define materials:
    double densityOfAluminum = 2.7;  // g/cm^3
    GeoMaterial *Aluminum = new GeoMaterial("Aluminum", densityOfAluminum);
    Aluminum->add(aluminum, 1.0);
    Aluminum->lock();

    // define two shape nodes
    const GeoBox *sh1 = new GeoBox(5.0, 5.0, 5.0);
    const GeoBox *sh2 = new GeoBox(1.0, 1.0, 1.0);

    // define two LogVol nodes
    const GeoLogVol *lv1 = new GeoLogVol("LV1", sh1, Aluminum);
    const GeoLogVol *lv2 = new GeoLogVol("LV2", sh2, Aluminum);

    // build two PhysVol volumes with the two LogVol nodes
    GeoPhysVol *pv1 = new GeoPhysVol(lv1);  // parent volume
    GeoPhysVol *pv2 = new GeoPhysVol(lv2);  // child volume

    // create the transforms to place the parent volumes
    GeoTransform *tr1 = new GeoTransform(GeoTrf::Translate3D(-15.0, 0, 0));
    GeoTransform *tr2 = new GeoTransform(GeoTrf::Translate3D(15.0, 0, 0));
    // create the transforms to place the child volumes inside the parent volume
    GeoTransform *tr3 = new GeoTransform(GeoTrf::Translate3D(-3.0, 0, 0));
    GeoTransform *tr4 = new GeoTransform(GeoTrf::Translate3D(3.0, 0, 0));

    // add the first vol to the world, at 0,0,0
    world->add(pv1);

    // add the same vol to the world, but this time at -15,0,0
    world->add(tr1);
    world->add(pv1);

    // add the same vol to the world, but this time at 15,0,0
    world->add(tr2);
    world->add(pv1);

    // add the child volume to the PV1 volume, at 0,0,0
    pv1->add(pv2);

    // add the same child volume to the PV1 volume, at -3,0,0
    pv1->add(tr3);
    pv1->add(pv2);

    // add the same child volume to the PV1 volume, at 3,0,0
    pv1->add(tr4);
    pv1->add(pv2);

    // --- I/O TESTS ---
    std::cout << "\nTests:\n";
    bool testall = GeoModelIO::TestIO::runAllTests(world, 0, 1, true).first;
    if (testall) return 0;
    return 1;
    // ----------------
}
