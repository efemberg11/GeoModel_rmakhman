/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Box.h
 *
 * Created on: May 7, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BUILDGEOSHAPES_BOX_H
#define GEOMODELREAD_BUILDGEOSHAPES_BOX_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>

class BuildGeoShapes_Box : public BuildGeoShapes
{
public:
  BuildGeoShapes_Box(const unsigned size):BuildGeoShapes("Box", size){};
  void buildShape(const DBRowEntry row) override;
};

#endif
