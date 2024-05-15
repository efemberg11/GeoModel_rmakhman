/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_Trd.h"

#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_Trd::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
  // === get shape numeric data from the DB row
  // shape ID
  const int shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "Trd:shapeID");
  // shape volume
  const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "Trd:shapeVolume");
  // shape parameters
  double XHalfLength1 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "Trd:XHalfLength1");
  double XHalfLength2 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Trd:XHalfLength2");
  double YHalfLength1 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Trd:YHalfLength1");
  double YHalfLength2 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[5], "Trd:YHalfLength2");
  double ZHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[6], "Trd:ZHalfLength");
  
  GeoShape *shape = new GeoTrd(XHalfLength1, XHalfLength2, YHalfLength1, YHalfLength2, ZHalfLength);

  storeBuiltShape(shapeId, shape);

  return;
}
