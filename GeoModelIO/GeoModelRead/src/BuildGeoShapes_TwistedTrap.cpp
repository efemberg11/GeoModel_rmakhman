/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_TwistedTrap.h"

#include "GeoModelKernel/GeoTwistedTrap.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_TwistedTrap::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
  // === get shape numeric data from the DB row
  // shape ID
  const int shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "TwistedTrap:shapeID");
  // shape volume
  const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "TwistedTrap:shapeVolume");
  // shape parameters
  double PhiTwist = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "TwistedTrap:PhiTwist");
  double ZHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "TwistedTrap:ZHalfLength");
  double Theta = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "TwistedTrap:Theta");
  double Phi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[5], "TwistedTrap:Phi");
  double DY1HalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[6], "TwistedTrap:DY1HalfLength");
  double DX1HalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[7], "TwistedTrap:DX1HalfLength");
  double DX2HalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[8], "TwistedTrap:DX2HalfLength");
  double DY2HalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[9], "TwistedTrap:DY2HalfLength");
  double DX3HalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[10], "TwistedTrap:DX3HalfLength");
  double DX4HalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[11], "TwistedTrap:DX4HalfLength");
  double DTiltAngleAlpha = GeoModelHelpers::variantHelper::getFromVariant_Double(row[12], "TwistedTrap:DTiltAngleAlpha");

  GeoShape *shape =
      new GeoTwistedTrap(PhiTwist, ZHalfLength, Theta, Phi, DY1HalfLength,
                         DX1HalfLength, DX2HalfLength, DY2HalfLength,
                         DX3HalfLength, DX4HalfLength, DTiltAngleAlpha);

  storeBuiltShape(shapeId, shape);

  return;
}
