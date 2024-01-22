/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMDOELFUNCSNPPETS_PRINTSHAPE_H
#define GEOMDOELFUNCSNPPETS_PRINTSHAPE_H
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include <string>

std::string printGeoShape(const GeoShape* shape);


/// Returns the sub operators of a GeoShape (GeoShapeUnion, GeoShapeSubtraction, GeoShapeUnion, GeoShapeShift)
std::pair<const GeoShape* , const GeoShape*> getOps(const GeoShape* composed);
/// Counts out of how many shapes the shape object is made up
unsigned int countComposedShapes(const GeoShape* shape);


/// @brief  If a the operating shape of a ShapeShift is another ShapeShift a
///         new shapeShift is created with the combined transformation 
GeoIntrusivePtr<const GeoShape> compressShift(const GeoShape* shift);

/// @brief Returns all acting of coomponents of a BooleanVolume. 
///        I.e. in case Of GeoShapeInterSection & GeoShapeUnion all building shape volumes
///        In the case of the GeoSubtraction all subtracting shapes
std::vector<const GeoShape*> getBooleanComponents(const GeoShape* booleanShape);



#endif