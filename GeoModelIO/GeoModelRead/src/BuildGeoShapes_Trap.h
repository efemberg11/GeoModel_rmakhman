/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Trap.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Trap_H
#define GEOMODELREAD_BuildGeoShapes_Trap_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>
#include <string>

class BuildGeoShapes_Trap : public BuildGeoShapes
{
public:
  BuildGeoShapes_Trap(const unsigned size):BuildGeoShapes("Trap", size){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
