/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>

const std::string GeoTubs::s_classType = "Tubs";
const ShapeType GeoTubs::s_classTypeID = 0x18;

GeoTubs::GeoTubs (double RMin, double RMax, double ZHalfLength, double SPhi, double DPhi)
  : m_rMin (RMin)
  , m_rMax (RMax)
  , m_zHalfLength (ZHalfLength)
  , m_sPhi (SPhi)
  , m_dPhi (DPhi)
{
}


double GeoTubs::volume () const
{
  return m_dPhi * (m_rMax * m_rMax - m_rMin * m_rMin) * m_zHalfLength;
}

void GeoTubs::extent (double& xmin, double& ymin, double& zmin,
                      double& xmax, double& ymax, double& zmax) const
{
  GeoShape::diskExtent(m_rMin, m_rMax, m_sPhi, m_dPhi, xmin, ymin, xmax, ymax);
  zmin =-m_zHalfLength;
  zmax = m_zHalfLength;
}

bool GeoTubs::contains (double x, double y, double z) const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  if (std::abs(z) - m_zHalfLength > 0.0) return false;
  double rr = x * x + y * y;
  if (rr > m_rMax * m_rMax || rr < m_rMin * m_rMin) return false;
  if (m_dPhi >= 2.0 * M_PI) return true;

  GeoTrf::Vector2D r(x, y);
  GeoTrf::Vector2D ns(std::sin(m_sPhi), -std::cos(m_sPhi));
  GeoTrf::Vector2D ne(-std::sin(m_sPhi + m_dPhi), std::cos(m_sPhi + m_dPhi));
  double ds = ns.dot(r);
  double de = ne.dot(r);
  return (m_dPhi <= M_PI) ? (ds <= 0 && de <= 0) : (ds <= 0 || de <= 0);
}


void GeoTubs::exec (GeoShapeAction *action) const
{
  action->handleTubs(this);
}
