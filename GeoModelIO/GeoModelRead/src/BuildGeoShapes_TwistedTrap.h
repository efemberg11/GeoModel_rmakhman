/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_TwistedTrap.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_TwistedTrap_H
#define GEOMODELREAD_BuildGeoShapes_TwistedTrap_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>

class BuildGeoShapes_TwistedTrap : public BuildGeoShapes
{
public:
  BuildGeoShapes_TwistedTrap(const unsigned size):BuildGeoShapes("TwistedTrap", size){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
