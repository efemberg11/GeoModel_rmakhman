/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoPolyhedrizeAction.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>

const std::string GeoTube::s_classType = "Tube";
const ShapeType GeoTube::s_classTypeID = 0x17;

GeoTube::GeoTube (double RMin, double RMax, double ZHalfLength)
  : m_rMin (RMin)
  , m_rMax (RMax)
  , m_zHalfLength (ZHalfLength)
{
}

GeoTube::~GeoTube()
{
}

double GeoTube::volume () const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  return 2.0 * M_PI * (m_rMax * m_rMax - m_rMin * m_rMin) * m_zHalfLength;
}

void GeoTube::extent (double& xmin, double& ymin, double& zmin,
                      double& xmax, double& ymax, double& zmax) const
{
  xmin =-m_rMax;
  ymin =-m_rMax;
  zmin =-m_zHalfLength;
  xmax = m_rMax;
  ymax = m_rMax;
  zmax = m_zHalfLength;
}

bool GeoTube::contains (double x, double y, double z) const
{
  if (std::abs(z) - m_zHalfLength > 0.0) return false;
  double rr = x * x + y * y;
  return ((rr <= m_rMax * m_rMax) && (rr >= m_rMin * m_rMin));
}

void GeoTube::exec (GeoShapeAction *action) const
{
  action->handleTube(this);
}
