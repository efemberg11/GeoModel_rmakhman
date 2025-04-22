/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoShapes_Pcon.h
 *
 * Created on: May 7, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 */

#ifndef GEOMODELREAD_BuildGeoShapes_Pcon_H
#define GEOMODELREAD_BuildGeoShapes_Pcon_H

#include "BuildGeoShapes.h"

#include "GeoModelDBManager/definitions.h"

#include <vector>
#include <variant>

class BuildGeoShapes_Pcon : public BuildGeoShapes
{
public:
  BuildGeoShapes_Pcon(const unsigned size, DBRowsList shapeData):BuildGeoShapes("Pcon", size, shapeData){};
  void buildShape(const DBRowEntry row) override;
};

#endif
