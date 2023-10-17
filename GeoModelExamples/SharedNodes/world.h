
// GeoModel includes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoTube.h"

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS \
    GeoModelKernelUnits  // so we will get, e.g., 'GeoModelKernelUnits::cm'
// Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration

/*
 * This class implements a default "World" volume.
 * It can be used to contain other pieces of geometry.
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Dec, 2022
 *
 */
// C++ includes
#include <iostream>

namespace GeoModel {

class World {
   public:
    World() {
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
        GeoElement* Argon =
            new GeoElement("Argon", "Ar", 18.0, 39.948 * gr / mole);
        GeoElement* Hydrogen =
            new GeoElement("Hydrogen", "H", 1.0, 1.00797 * gr / mole);

        // Define the materials

        double densityOfAir = 0.001214 * gr / cm3;
        GeoMaterial* air = new GeoMaterial("Air", densityOfAir);
        air->add(Nitrogen, 0.7494);
        air->add(Oxygen, 0.2369);
        air->add(Argon, 0.0129);
        air->add(Hydrogen, 0.0008);
        air->lock();

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
        m_pv = world;
    }

    GeoPhysVol* getPhysVol() { return m_pv; }

   private:
    GeoPhysVol* m_pv;

};  // close class

}  // namespace GeoModel

