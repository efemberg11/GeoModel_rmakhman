/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_Trap.h"

#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_Trap::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
  // === get shape numeric data from the DB row
  // shape ID
  const int shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "Trap:shapeID");
  // shape volume
  const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "Trap:shapeVolume");
  // shape parameters
  double ZHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "Trap:ZHalfLength");
  double Theta = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Trap:Theta");
  double Phi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Trap:Phi");
  double Dydzn = GeoModelHelpers::variantHelper::getFromVariant_Double(row[5], "Trap:Dydzn");
  double Dxdyndzn = GeoModelHelpers::variantHelper::getFromVariant_Double(row[6], "Trap:Dxdyndzn");
  double Dxdypdzn = GeoModelHelpers::variantHelper::getFromVariant_Double(row[7], "Trap:Dxdypdzn");
  double Angleydzn = GeoModelHelpers::variantHelper::getFromVariant_Double(row[8], "Trap:Angleydzn");
  double Dydzp = GeoModelHelpers::variantHelper::getFromVariant_Double(row[9], "Trap:Dydzp");
  double Dxdyndzp = GeoModelHelpers::variantHelper::getFromVariant_Double(row[10], "Trap:Dxdyndzp");
  double Dxdypdzp = GeoModelHelpers::variantHelper::getFromVariant_Double(row[11], "Trap:Dxdypdzp");
  double Angleydzp = GeoModelHelpers::variantHelper::getFromVariant_Double(row[12], "Trap:Angleydzp");

  GeoShape *shape = new GeoTrap(ZHalfLength, Theta, Phi, Dydzn, Dxdyndzn, Dxdypdzn,
                                Angleydzn, Dydzp, Dxdyndzp, Dxdypdzp, Angleydzp);

  storeBuiltShape(shapeId, shape);

  return;
}
