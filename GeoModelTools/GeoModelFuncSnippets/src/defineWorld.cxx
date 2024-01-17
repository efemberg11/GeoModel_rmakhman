/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelFuncSnippets/defineWorld.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/Units.h"

GeoIntrusivePtr<GeoPhysVol> createGeoWorld(const double worldBoxX, 
                                           const double worldBoxY,
                                           const double worldBoxZ) {
    

    constexpr double gr =   GeoModelKernelUnits::gram;
    constexpr double mole = GeoModelKernelUnits::mole;
    constexpr double cm3 =  GeoModelKernelUnits::cm3;

    // Define the chemical elements
    GeoIntrusivePtr<GeoElement>  Nitrogen{new GeoElement ("Nitrogen" ,"N"  ,  7.0 ,  14.0067 *gr/mole)};
    GeoIntrusivePtr<GeoElement>  Oxygen{new GeoElement ("Oxygen"   ,"O"  ,  8.0 ,  15.9995 *gr/mole)};
    GeoIntrusivePtr<GeoElement>  Argon{new GeoElement ("Argon"    ,"Ar" , 18.0 ,  39.948  *gr/mole)};
    GeoIntrusivePtr<GeoElement>  Hydrogen{new GeoElement ("Hydrogen" ,"H"  ,  1.0 ,  1.00797 *gr/mole)};

    constexpr double densityOfAir=0.001214 *gr/cm3;
    GeoIntrusivePtr<GeoMaterial> air{new GeoMaterial("Air", densityOfAir)};
    air->add(Nitrogen  , 0.7494);
    air->add(Oxygen, 0.2369);
    air->add(Argon, 0.0129);
    air->add(Hydrogen, 0.0008);
    air->lock();



    GeoIntrusivePtr<GeoBox> worldBox{new GeoBox(worldBoxX, worldBoxY, worldBoxZ)};
    GeoIntrusivePtr<GeoLogVol> worldLog{new GeoLogVol("WorldLog", worldBox, air)};
    GeoIntrusivePtr<GeoPhysVol> world{new GeoPhysVol(worldLog)};

    return world;    
}