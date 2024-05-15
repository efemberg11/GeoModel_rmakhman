/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Trd.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Trd_H
#define GEOMODELREAD_BuildGeoShapes_Trd_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>
#include <string>

class BuildGeoShapes_Trd : public BuildGeoShapes
{
public:
  BuildGeoShapes_Trd(const unsigned size):BuildGeoShapes("Trd", size){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
