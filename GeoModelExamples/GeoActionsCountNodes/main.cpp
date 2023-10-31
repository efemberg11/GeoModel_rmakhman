// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

/*
 * main.cpp
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Jan, 2023
 *
 */

#include <iostream>

#include "CountNodesGraphAction.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoBox.h"

#include <map>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS \
    GeoModelKernelUnits  // so we will get, e.g., 'GeoModelKernelUnits::cm'

int main(int argc, char* argv[]) {
    //-----------------------------------------------------------------------------------//
    // Define the materials that we shall use. //
    // ----------------------------------------------------------------------------------//

// Define the units
#define gr SYSTEM_OF_UNITS::gram
#define mole SYSTEM_OF_UNITS::mole
#define cm3 SYSTEM_OF_UNITS::cm3

    // Define the chemical elements
    GeoElement* Nitrogen =
        new GeoElement("Nitrogen", "N", 7.0, 14.0067 * gr / mole);
    GeoElement* Oxygen =
        new GeoElement("Oxygen", "O", 8.0, 15.9995 * gr / mole);
    GeoElement* Argon = new GeoElement("Argon", "Ar", 18.0, 39.948 * gr / mole);
    GeoElement* Hydrogen =
        new GeoElement("Hydrogen", "H", 1.0, 1.00797 * gr / mole);
    GeoElement* Iron = new GeoElement("Iron", "Fe", 26.0, 55.847 * gr / mole);
    GeoElement* Carbon =
        new GeoElement("Carbon", "C", 6.0, 12.0107 * gr / mole);
    GeoElement* Sillicon =
        new GeoElement("Silicon", "Si", 14.0, 28.085 * gr / mole);

    // Define the materials

    double densityOfAir = 0.001214 * gr / cm3;
    GeoMaterial* air = new GeoMaterial("Air", densityOfAir);
    air->add(Nitrogen, 0.7494);
    air->add(Oxygen, 0.2369);
    air->add(Argon, 0.0129);
    air->add(Hydrogen, 0.0008);
    air->lock();

    GeoMaterial* steel = new GeoMaterial("Steel", 7.9 * gr / cm3);
    steel->add(Iron, 0.98);
    steel->add(Carbon, 0.02);
    steel->lock();

    // Silicon 100% (Detector)
    GeoMaterial* silicon = new GeoMaterial("Silicon", 2.329 * gr / cm3);
    silicon->add(const_cast<GeoElement*>(Sillicon), 1.0);
    silicon->lock();

    //-----------------------------------------------------------------------------------//
    // create the world volume container and
    // get the 'world' volume, i.e. the root volume of the GeoModel tree
    std::cout << "Creating the 'world' volume, i.e. the root volume of the "
                 "GeoModel tree..."
              << std::endl;
    const GeoBox* worldBox =
        new GeoBox(1000 * SYSTEM_OF_UNITS::cm, 1000 * SYSTEM_OF_UNITS::cm,
                   1000 * SYSTEM_OF_UNITS::cm);
    const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
    GeoPhysVol* world = new GeoPhysVol(worldLog);

    //-----------------------------------------------------------------------------------//
    // Next make the box that describes the shape of the toy volume: //
    const GeoBox* toyBox =
        new GeoBox(800 * SYSTEM_OF_UNITS::cm, 800 * SYSTEM_OF_UNITS::cm,
                   1000 * SYSTEM_OF_UNITS::cm);  //
    // Bundle this with a material into a logical volume: //
    const GeoLogVol* toyLog = new GeoLogVol("ToyLog", toyBox, silicon);  //
    // ..And create a physical volume: //
    GeoPhysVol* toyPhys = new GeoPhysVol(toyLog);  //

    GeoBox* sPass =
        new GeoBox(5.0 * SYSTEM_OF_UNITS::cm, 30 * SYSTEM_OF_UNITS::cm,
                   30 * SYSTEM_OF_UNITS::cm);
    GeoLogVol* lPass = new GeoLogVol("Passive", sPass, steel);
    GeoPhysVol* pPass = new GeoPhysVol(lPass);

    GeoBox* sIPass =
        new GeoBox(4 * SYSTEM_OF_UNITS::cm, 25 * SYSTEM_OF_UNITS::cm,
                   25 * SYSTEM_OF_UNITS::cm);
    GeoLogVol* lIPass = new GeoLogVol("InnerPassive", sIPass, air);
    GeoPhysVol* pIPass = new GeoPhysVol(lIPass);

    pPass->add(pIPass);
    toyPhys->add(pPass);

    //------------------------------------------------------------------------------------//
    // Now insert all of this into the world... //
    GeoNameTag* tag = new GeoNameTag("Toy");  //
    world->add(tag);                          //
    world->add(toyPhys);                      //
    //------------------------------------------------------------------------------------//

    std::cout
        << "\n*** Custom GeoVolumeAction example to count given types of nodes: ***"
        << std::endl;

    std::map<std::string, unsigned long> mmap;
    // Dump the tree volumes to a local file
    std::cout
        << "Inspecting the GeoModel tree starting from the input volume..."
        << std::endl;
    // init the graph action
    CountNodesGraphAction countNodes;
    world->exec(&countNodes);  // visit all GeoModel nodes

    // countin nodes (can be expanded to all types of nodes)
    unsigned long nphysvols = countNodes.countPhysVols();
    unsigned long nlogvols = countNodes.countLogVols();
    unsigned long nshapes = countNodes.countShapes();
    unsigned long nmaterials = countNodes.countMaterials();
    mmap["PhysVol"] = nphysvols;
    mmap["Material"] = nmaterials;
    mmap["LogVol"] = nlogvols;
    mmap["Shapes"] = nshapes;

    for (auto& nodetype : mmap) {
        std::cout << nodetype.first << ": " << nodetype.second << std::endl;
    }
    std::cout << "DONE. GeoModel tree inspected." << std::endl;

    return 0;
}
