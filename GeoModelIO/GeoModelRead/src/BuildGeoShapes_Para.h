/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Para.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Para_H
#define GEOMODELREAD_BuildGeoShapes_Para_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>
#include <string>

class BuildGeoShapes_Para : public BuildGeoShapes
{
public:
  BuildGeoShapes_Para(const unsigned size):BuildGeoShapes("Para", size){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
