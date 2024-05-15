/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_SimplePolygonBrep.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_SimplePolygonBrep_H
#define GEOMODELREAD_BuildGeoShapes_SimplePolygonBrep_H

#include "BuildGeoShapes.h"

#include "GeoModelDBManager/definitions.h"

#include <vector>
#include <variant>

class BuildGeoShapes_SimplePolygonBrep : public BuildGeoShapes
{
public:
  BuildGeoShapes_SimplePolygonBrep(const unsigned size, DBRowsList shapeData):BuildGeoShapes("SimplePolygonBrep", size, shapeData){};
  void buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row) override;
};

#endif
