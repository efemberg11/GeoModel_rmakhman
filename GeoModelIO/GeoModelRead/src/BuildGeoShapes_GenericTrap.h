/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_GenericTrap.h
 *
 * Created on: June, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_GenericTrap_H
#define GEOMODELREAD_BuildGeoShapes_GenericTrap_H

#include "BuildGeoShapes.h"

#include "GeoModelDBManager/definitions.h"

#include <vector>
#include <variant>

class BuildGeoShapes_GenericTrap : public BuildGeoShapes
{
public:
  BuildGeoShapes_GenericTrap(const unsigned size, DBRowsList shapeData):BuildGeoShapes("GenericTrap", size, shapeData){};
  void buildShape(const DBRowEntry row) override;
};

#endif
