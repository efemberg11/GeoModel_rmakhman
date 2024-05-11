/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May 7, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_Cons.h"

#include "GeoModelKernel/GeoCons.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_Cons::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
    // === get shape numeric data from the DB row
    // shape ID
    const int shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "Cons:shapeID");
    // shape volume
    const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "Cons:shapeVolume");
    // shape parameters
    const double RMin1 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "Cons:RMin1");
    const double RMin2 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Cons:RMin2");
    const double RMax1 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Cons:RMax1");
    const double RMax2 = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Cons:RMax2");
    const double DZ = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Cons:DZ");
    const double SPhi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Cons:SPhi");
    const double DPhi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Cons:DPhi");

    GeoCons *shape = new GeoCons(RMin1, RMin2, RMax1, RMax2, DZ, SPhi, DPhi);

    storeBuiltShape(shapeId, shape);

    return;
}
