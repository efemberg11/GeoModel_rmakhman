/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Cons.h
 *
 * Created on: May 7, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Cons_H
#define GEOMODELREAD_BuildGeoShapes_Cons_H

#include "BuildGeoShapes.h"

#include <vector>
#include <variant>
#include <string>

class BuildGeoShapes_Cons : public BuildGeoShapes
{
public:
  BuildGeoShapes_Cons(const unsigned size):BuildGeoShapes("Cons", size){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
