/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include <algorithm>
#include <cmath>
#include <cstdint>

double GeoShape::volume (int npoints) const
{
  constexpr double expansion = 0.001;  // bounding box expansion
  constexpr double f = 1./4294967296.; // 2^-32 - int to double conversion
  int np = std::max(npoints, 1000);    // number of points is at least 1000

  // set up bonding box
  double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
  extent(xmin, ymin, zmin, xmax, ymax, zmax);
  double delx = (xmax - xmin) * expansion;
  double dely = (ymax - ymin) * expansion;
  double delz = (zmax - zmin) * expansion;
  xmin -= delx;
  ymin -= dely;
  zmin -= delz;
  xmax += delx;
  ymax += dely;
  zmax += delz;

  uint32_t y = 2463534242; // seed for random number generation
  int icount = 0; // counter of inside points
  for (int i = 0; i < np; ++i)
  {
    // generate three random numbers
    uint32_t x = y;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    double randx = x * f;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    double randy = x * f;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    double randz = x * f;
    y = x;

    // calculate coordinates of random point and check its position
    double px = xmin + (xmax - xmin) * randx;
    double py = ymin + (ymax - ymin) * randy;
    double pz = zmin + (zmax - zmin) * randz;
    icount += contains(px, py, pz);
  }
  return (xmax - xmin) * (ymax - ymin) * (zmax -zmin) * icount / npoints;
}

const GeoShapeUnion & GeoShape::add (const GeoShape& shape) const
{
  GeoShapeUnion *unionNode = new GeoShapeUnion (this, &shape);
  return *unionNode;
}

const GeoShapeSubtraction & GeoShape::subtract (const GeoShape& shape) const
{
  GeoShapeSubtraction *subNode = new GeoShapeSubtraction (this, &shape);
  return *subNode;
}

const GeoShapeIntersection & GeoShape::intersect (const GeoShape& shape) const
{
  GeoShapeIntersection *intNode = new GeoShapeIntersection (this, &shape);
  return *intNode;
}

const GeoShapeShift & GeoShape::operator << (const GeoTrf::Transform3D &shift) const
{
  GeoShapeShift *shiftNode = new GeoShapeShift (this, shift);
  return *shiftNode;
}

void GeoShape::diskExtent(double rmin, double rmax, double sphi, double dphi,
                          double& xmin, double& ymin, double& xmax, double& ymax)
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  xmin = ymin =-rmax;
  xmax = ymax = rmax;
  if (dphi >= 2. * M_PI) return;

  double sinStart = std::sin(sphi);
  double cosStart = std::cos(sphi);
  double sinEnd = std::sin(sphi + dphi);
  double cosEnd = std::cos(sphi + dphi);

  // get start and end quadrants, sixteen possible cases
  //
  //      1 | 0
  //     ---+---
  //      3 | 2
  //
  int icase = (cosEnd < 0) ? 1 : 0;
  if (sinEnd   < 0) icase += 2;
  if (cosStart < 0) icase += 4;
  if (sinStart < 0) icase += 8;

  switch (icase)
  {
  // start quadrant 0
  case  0:                       // start->end : 0->0
    if (sinEnd < sinStart) break;
    xmin = rmin*cosEnd;
    ymin = rmin*sinStart;
    xmax = rmax*cosStart;
    ymax = rmax*sinEnd;
    break;
  case  1:                       // start->end : 0->1
    xmin = rmax*cosEnd;
    ymin = std::min(rmin*sinStart, rmin*sinEnd);
    xmax = rmax*cosStart;
    ymax = rmax;
    break;
  case  2:                       // start->end : 0->2
    xmin =-rmax;
    ymin =-rmax;
    xmax = std::max(rmax*cosStart, rmax*cosEnd);
    ymax = rmax;
    break;
  case  3:                       // start->end : 0->3
    xmin =-rmax;
    ymin = rmax*sinEnd;
    xmax = rmax*cosStart;
    ymax = rmax;
    break;
  // start quadrant 1
  case  4:                       // start->end : 1->0
    xmin =-rmax;
    ymin =-rmax;
    xmax = rmax;
    ymax = std::max(rmax*sinStart, rmax*sinEnd);
    break;
  case  5:                       // start->end : 1->1
    if (sinEnd > sinStart) break;
    xmin = rmax*cosEnd;
    ymin = rmin*sinEnd;
    xmax = rmin*cosStart;
    ymax = rmax*sinStart;
    break;
  case  6:                       // start->end : 1->2
    xmin =-rmax;
    ymin =-rmax;
    xmax = rmax*cosEnd;
    ymax = rmax*sinStart;
    break;
  case  7:                       // start->end : 1->3
    xmin =-rmax;
    ymin = rmax*sinEnd;
    xmax = std::max(rmin*cosStart, rmin*cosEnd);
    ymax = rmax*sinStart;
    break;
  // start quadrant 2
  case  8:                       // start->end : 2->0
    xmin = std::min(rmin*cosStart, rmin*cosEnd);
    ymin = rmax*sinStart;
    xmax = rmax;
    ymax = rmax*sinEnd;
    break;
  case  9:                       // start->end : 2->1
    xmin = rmax*cosEnd;
    ymin = rmax*sinStart;
    xmax = rmax;
    ymax = rmax;
    break;
  case 10:                       // start->end : 2->2
    if (sinEnd < sinStart) break;
    xmin = rmin*cosStart;
    ymin = rmax*sinStart;
    xmax = rmax*cosEnd;
    ymax = rmin*sinEnd;
    break;
  case 11:                       // start->end : 2->3
    xmin =-rmax;
    ymin = std::min(rmax*sinStart, rmax*sinEnd);
    xmax = rmax;
    ymax = rmax;
    break;
  // start quadrant 3
  case 12:                       // start->end : 3->0
    xmin = rmax*cosStart;
    ymin =-rmax;
    xmax = rmax;
    ymax = rmax*sinEnd;
    break;
  case 13:                       // start->end : 3->1
    xmin = std::min(rmax*cosStart, rmax*cosEnd);
    ymin =-rmax;
    xmax = rmax;
    ymax = rmax;
    break;
  case 14:                       // start->end : 3->2
    xmin = rmax*cosStart;
    ymin =-rmax;
    xmax = rmax*cosEnd;
    ymax = std::max(rmin*sinStart, rmin*sinEnd);
    break;
  case 15:                       // start->end : 3->3
    if (sinEnd > sinStart) break;
    xmin = rmax*cosStart;
    ymin = rmax*sinEnd;
    xmax = rmin*cosEnd;
    ymax = rmin*sinStart;
    break;
  }
}
