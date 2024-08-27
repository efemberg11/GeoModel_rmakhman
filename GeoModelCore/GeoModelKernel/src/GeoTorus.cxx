/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>

const std::string GeoTorus::s_classType = "Torus";
const ShapeType GeoTorus::s_classTypeID = 0x24;

GeoTorus::GeoTorus (double Rmin, double Rmax, double Rtor, double SPhi, double DPhi)
   : m_rMin (Rmin)
   , m_rMax (Rmax)
   , m_rTor (Rtor)
   , m_sPhi (SPhi)
   , m_dPhi (DPhi)
{}

double GeoTorus::volume (int) const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  return m_dPhi * M_PI * m_rTor *(m_rMax * m_rMax - m_rMin * m_rMin);
}

void GeoTorus::extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const
{
  double rmin = m_rTor - m_rMax;
  double rmax = m_rTor + m_rMax;
  GeoShape::diskExtent(rmin, rmax, m_sPhi, m_dPhi, xmin, ymin, xmax, ymax);
  zmin =-m_rMax;
  zmax = m_rMax;
}

bool GeoTorus::contains (double x, double y, double z) const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  double rr = x * x + y * y;
  if (std::abs(z) - m_rMax > 0.0 || rr == 0.0) return false;
  double rmin = m_rTor - m_rMax;
  double rmax = m_rTor + m_rMax;
  if (rr > rmax * rmax || rr < rmin * rmin) return false;

  double t = (1.0 - m_rTor / std::sqrt(rr));
  double xt = x * t;
  double yt = y * t;
  rr = xt * xt + yt * yt + z * z;
  if (rr > m_rMax * m_rMax || rr < m_rMin * m_rMin) return false;
  if (m_dPhi >= 2.0 * M_PI) return true;

  GeoTrf::Vector2D r(x, y);
  GeoTrf::Vector2D ns(std::sin(m_sPhi), -std::cos(m_sPhi));
  GeoTrf::Vector2D ne(-std::sin(m_sPhi + m_dPhi), std::cos(m_sPhi + m_dPhi));
  double ds = ns.dot(r);
  double de = ne.dot(r);
  return (m_dPhi <= M_PI) ? (ds <= 0 && de <= 0) : (ds <= 0 || de <= 0);
}

void GeoTorus::exec (GeoShapeAction *action) const
{
  action->handleTorus(this);
}
