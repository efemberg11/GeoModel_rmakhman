/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_Para.h"

#include "GeoModelKernel/GeoPara.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_Para::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
  // === get shape numeric data from the DB row
  // shape ID
  const int shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "Para:shapeID");
  // shape volume
  const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "Para:shapeVolume");

  // shape parameters
  double XHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "Para:XHalfLength");
  double YHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Para:YHalfLength");
  double ZHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Para:ZHalfLength");
  double Alpha = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Para:Alpha");
  double Theta = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Para:Theta");
  double Phi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Para:Phi");

  GeoPara *shape = new GeoPara(XHalfLength, YHalfLength, ZHalfLength, Alpha, Theta,
                               Phi);
  storeBuiltShape(shapeId, shape);

  return;
}
