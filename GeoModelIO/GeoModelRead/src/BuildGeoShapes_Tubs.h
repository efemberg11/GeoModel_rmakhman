/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Tubs.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Tubs_H
#define GEOMODELREAD_BuildGeoShapes_Tubs_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>

class BuildGeoShapes_Tubs : public BuildGeoShapes
{
public:
  BuildGeoShapes_Tubs(const unsigned size):BuildGeoShapes("Tubs", size){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
