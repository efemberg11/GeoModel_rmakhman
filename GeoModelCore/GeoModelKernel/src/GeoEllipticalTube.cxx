/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoPolyhedrizeAction.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>

#include "GeoModelKernel/GeoEllipticalTube.h"
const std::string GeoEllipticalTube::s_classType = "EllipticalTube";
const ShapeType GeoEllipticalTube::s_classTypeID = 0x22;

GeoEllipticalTube::GeoEllipticalTube(double XHalfLength, double YHalfLength, double ZHalfLength)
  : m_xHalfLength(XHalfLength)
  , m_yHalfLength(YHalfLength)
  , m_zHalfLength(ZHalfLength)
{
}

GeoEllipticalTube::~GeoEllipticalTube()
{
}

double GeoEllipticalTube::volume () const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  return 2.0 * M_PI * m_xHalfLength * m_yHalfLength * m_zHalfLength;
}

void GeoEllipticalTube::extent (double& xmin, double& ymin, double& zmin,
                                double& xmax, double& ymax, double& zmax) const
{
  xmin =-m_xHalfLength;
  ymin =-m_yHalfLength;
  zmin =-m_zHalfLength;
  xmax = m_xHalfLength;
  ymax = m_yHalfLength;
  zmax = m_zHalfLength;
}

bool GeoEllipticalTube::contains (double x, double y, double z) const
{
  if (std::abs(z) - m_zHalfLength > 0.0) return false;
  return ((x * x) / (m_xHalfLength * m_xHalfLength) +
          (y * y) / (m_yHalfLength * m_yHalfLength) <= 1.0);
}

const std::string & GeoEllipticalTube::type () const
{
  return s_classType;
}

ShapeType GeoEllipticalTube::typeID () const
{
  return s_classTypeID;
}

void GeoEllipticalTube::exec (GeoShapeAction *action) const
{
  action->handleEllipticalTube(this);
}
