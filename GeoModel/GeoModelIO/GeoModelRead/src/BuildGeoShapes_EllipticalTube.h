/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_EllipticalTube.h
 *
 * Created on: Oct 3, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BUILDGEOSHAPES_EllipticalTube_H
#define GEOMODELREAD_BUILDGEOSHAPES_EllipticalTube_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>

class BuildGeoShapes_EllipticalTube : public BuildGeoShapes
{
public:
  BuildGeoShapes_EllipticalTube(const unsigned size):BuildGeoShapes("EllipticalTube", size){};
  void buildShape(const DBRowEntry row) override;
};

#endif
