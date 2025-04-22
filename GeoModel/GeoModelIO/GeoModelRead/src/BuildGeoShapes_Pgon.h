/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Pgon.h
 *
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Pgon_H
#define GEOMODELREAD_BuildGeoShapes_Pgon_H

#include "BuildGeoShapes.h"

#include "GeoModelDBManager/definitions.h"

#include <vector>
#include <variant>

class BuildGeoShapes_Pgon : public BuildGeoShapes
{
public:
  BuildGeoShapes_Pgon(const unsigned size, DBRowsList shapeData):BuildGeoShapes("Pgon", size, shapeData){};
  void buildShape(const DBRowEntry row) override;
};

#endif
