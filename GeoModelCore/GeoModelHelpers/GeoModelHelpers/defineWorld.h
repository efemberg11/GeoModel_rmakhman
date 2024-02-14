/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELUTILS_DEFINEWORLD_H
#define GEOMODELUTILS_DEFINEWORLD_H

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/Units.h"

/// @brief  Common helper function that creates the top GeoWorld volume in the tree.
/// @returns A physical volume serving as world volume
namespace GeoWorldDim {
    constexpr double worldBoxX = 20. * GeoModelKernelUnits::m;
    constexpr double worldBoxY = 20. * GeoModelKernelUnits::m;
    constexpr double worldBoxZ = 25. * GeoModelKernelUnits::m;
}

/// Creates the basic GeoWorld
GeoIntrusivePtr<GeoPhysVol> createGeoWorld(const double worldBoxX = GeoWorldDim::worldBoxX,
                                           const double worldBoxY = GeoWorldDim::worldBoxY,
                                           const double worldBoxZ = GeoWorldDim::worldBoxZ);

/// Resize the world volume to be the needed size
GeoIntrusivePtr<GeoPhysVol> resizeGeoWorld(GeoIntrusivePtr<GeoPhysVol> world);
#endif
