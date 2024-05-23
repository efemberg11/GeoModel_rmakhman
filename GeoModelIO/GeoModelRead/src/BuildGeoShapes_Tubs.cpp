/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_Tubs.h"

#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_Tubs::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
  // === get shape numeric data from the DB row
  // shape ID
  const int shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "Tubs:shapeID");
  // shape volume
  const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "Tubs:shapeVolume");
  // shape parameters
  const double RMin = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "Tubs:RMin");
  const double RMax = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Tubs:RMax");
  const double ZHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Tubs:ZHalfLength");
  const double SPhi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[5], "Tubs:SPhi");
  const double DPhi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[6], "Tubs:DPhi");
  
  GeoShape *shape = new GeoTubs(RMin, RMax, ZHalfLength, SPhi, DPhi);

  storeBuiltShape(shapeId, shape);

  return;
}
