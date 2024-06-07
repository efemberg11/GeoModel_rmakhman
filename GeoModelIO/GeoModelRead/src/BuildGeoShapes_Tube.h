/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Tube.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Tube_H
#define GEOMODELREAD_BuildGeoShapes_Tube_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>

class BuildGeoShapes_Tube : public BuildGeoShapes
{
public:
  BuildGeoShapes_Tube(const unsigned size):BuildGeoShapes("Tube", size){};
  void buildShape(const DBRowEntry row) override;
};

#endif
