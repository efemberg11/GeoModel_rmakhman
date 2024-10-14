/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoVSurfaces.h
 *
 * Created on: June, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */

#ifndef GEOMODELREAD_BUILDGEOVSURFACE_H
#define GEOMODELREAD_BUILDGEOVSURFACE_H

#include "GeoModelDBManager/definitions.h"
#include "GeoModelHelpers/variantHelpers.h"

#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/GeoTrapezoidSurface.h"
#include "GeoModelKernel/GeoAnnulusSurface.h"
#include "GeoModelKernel/GeoDiamondSurface.h"

#include <iostream>
#include <vector>
#include <variant>
#include <string>

class BuildGeoVSurface
{
public:

  BuildGeoVSurface() = default;
  GeoRectSurface* buildRectSurface(const DBRowEntry row);
  GeoTrapezoidSurface* buildTrapezoidSurface(const DBRowEntry row);
  GeoAnnulusSurface* buildAnnulusSurface(const DBRowEntry row);
  GeoDiamondSurface* buildDiamondSurface(const DBRowEntry row);
  // left for other surfaces
};

#endif
