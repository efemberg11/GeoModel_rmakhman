/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Torus.h
 *
 * Created on: June, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Torus_H
#define GEOMODELREAD_BuildGeoShapes_Torus_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>

class BuildGeoShapes_Torus : public BuildGeoShapes
{
public:
  BuildGeoShapes_Torus(const unsigned size):BuildGeoShapes("Torus", size){};
  void buildShape(const DBRowEntry row) override;
};

#endif
