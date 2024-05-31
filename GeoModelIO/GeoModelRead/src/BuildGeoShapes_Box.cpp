/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * Created on: May 7, 2024
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 */

#include "BuildGeoShapes_Box.h"

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelHelpers/variantHelpers.h"

#include <vector>
#include <iostream>

void BuildGeoShapes_Box::buildShape(const std::vector<std::variant<int, long, float, double, std::string>> row)
{
    // === get shape numeric data from the DB row
    // shape ID
    const unsigned shapeId = GeoModelHelpers::variantHelper::getFromVariant_Int(row[0], "Box:shapeID");
    // shape volume
    const double shapeVolume = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "Box:shapeVolume");
    // shape parameters
    const double XHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "Box:XHalfLength");
    const double YHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "Box:YHalfLength");
    const double ZHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "Box:ZHalfLength");

    GeoBox *shape = new GeoBox(XHalfLength, YHalfLength, ZHalfLength);

    storeBuiltShape(shapeId, shape);

    // return shape;
    return;
}
