/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 * BuildGeoVSurfaces.h
 *
 * Created on: June, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */
#include "BuildGeoVSurface.h"

GeoRectSurface* BuildGeoVSurface::buildRectSurface(const DBRowEntry row)
{
    // shape area
    const double surfArea = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "SurfaceRect_Area");
    // shape parameters
    const double XHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "SurfaceRect_XHalfLength");
    const double YHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "SurfaceRect_YHalfLength");

    GeoRectSurface *rectsurface = new GeoRectSurface(XHalfLength, YHalfLength);
    
    return rectsurface;
}

GeoTrapezoidSurface* BuildGeoVSurface::buildTrapezoidSurface(const DBRowEntry row)
{
    // shape area
    const double surfArea = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "SurfaceTrapezoid_Area");
    // shape parameters
    const double XHalfLengthMin = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "SurfaceTrapezoid_XHalfLengthMin");
    const double XHalfLengthMax = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "SurfaceTrapezoid_XHalfLengthMax");    
    const double YHalfLength = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "SurfaceTrapezoid_YHalfLength");

    GeoTrapezoidSurface *trapezoid = new GeoTrapezoidSurface(XHalfLengthMin, XHalfLengthMax, YHalfLength);
    
    return trapezoid;
}

GeoAnnulusSurface* BuildGeoVSurface::buildAnnulusSurface(const DBRowEntry row)
{
    // shape area
    const double surfArea = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "SurfaceAnnulus_Area");
    // shape parameters
    const double Ox = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "SurfaceAnnulus_Ox");
    const double Oy = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "SurfaceAnnulus_Oy");
    const double radius_in = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "SurfaceAnnulus_RadiusIn");
    const double radius_out = GeoModelHelpers::variantHelper::getFromVariant_Double(row[5], "SurfaceAnnulus_RadiusOut");
    const double phi = GeoModelHelpers::variantHelper::getFromVariant_Double(row[6], "SurfaceAnnulus_Phi");

    GeoAnnulusSurface *annulus = new GeoAnnulusSurface(Ox, Oy, radius_in, radius_out, phi);
    
    return annulus;
}

GeoDiamondSurface* BuildGeoVSurface::buildDiamondSurface(const DBRowEntry row)
{
    // shape area
    const double surfArea = GeoModelHelpers::variantHelper::getFromVariant_Double(row[1], "SurfaceDiamond_Area");
    // shape parameters
    const double X_bottom_half = GeoModelHelpers::variantHelper::getFromVariant_Double(row[2], "SurfaceDiamond_XBottomHalf");
    const double X_mid_half = GeoModelHelpers::variantHelper::getFromVariant_Double(row[3], "SurfaceDiamond_XMidHalf");
    const double X_top_half = GeoModelHelpers::variantHelper::getFromVariant_Double(row[4], "SurfaceDiamond_XTopHalf");
    const double Y_bottom_half = GeoModelHelpers::variantHelper::getFromVariant_Double(row[5], "SurfaceDiamond_YBottomHalf");
    const double Y_top_half = GeoModelHelpers::variantHelper::getFromVariant_Double(row[6], "SurfaceDiamond_YTopHalf");

    GeoDiamondSurface *diamond = new GeoDiamondSurface(X_bottom_half, X_mid_half, X_top_half, Y_bottom_half, Y_top_half);
    
    return diamond;
}