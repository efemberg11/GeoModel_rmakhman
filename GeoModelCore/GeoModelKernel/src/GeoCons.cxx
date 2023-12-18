/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>

const std::string GeoCons::s_classType = "Cons";
const ShapeType GeoCons::s_classTypeID = 0x11;

GeoCons::GeoCons (double RMin1, double RMin2, double RMax1, double RMax2, double DZ, double SPhi, double DPhi)
  : m_rMin1 (RMin1)
  , m_rMin2 (RMin2)
  , m_rMax1 (RMax1)
  , m_rMax2 (RMax2)
  , m_dZ (DZ)
  , m_sPhi (SPhi)
  , m_dPhi (DPhi)
{
}


double GeoCons::volume () const
{
  return (m_dPhi * (1./3.)) * m_dZ *
    (m_rMax1 * m_rMax1 + m_rMax2 * m_rMax2 + m_rMax1 * m_rMax2 -
     m_rMin1 * m_rMin1 - m_rMin2 * m_rMin2 - m_rMin1 * m_rMin2);
}

void GeoCons::extent (double& xmin, double& ymin, double& zmin,
                      double& xmax, double& ymax, double& zmax) const
{
  double rmin = std::min(m_rMin1, m_rMin2);
  double rmax = std::max(m_rMax1, m_rMax2);
  GeoShape::diskExtent(rmin, rmax, m_sPhi, m_dPhi, xmin, ymin, xmax, ymax);
  zmin =-m_dZ;
  zmax = m_dZ;
}

bool GeoCons::contains (double x, double y, double z) const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  if (std::abs(z) - m_dZ > 0.0) return false;
  double t = 0.5 * (1.0 + z / m_dZ);
  double rmin = m_rMin1 + (m_rMin2 - m_rMin1) * t;
  double rmax = m_rMax1 + (m_rMax2 - m_rMax1) * t;
  double rr = x * x + y * y;
  if (rr > rmax * rmax || rr < rmin * rmin) return false;
  if (m_dPhi >= 2.0 * M_PI) return true;

  GeoTrf::Vector2D r(x, y);
  GeoTrf::Vector2D ns(std::sin(m_sPhi), -std::cos(m_sPhi));
  GeoTrf::Vector2D ne(-std::sin(m_sPhi + m_dPhi), std::cos(m_sPhi + m_dPhi));
  double ds = ns.dot(r);
  double de = ne.dot(r);
  return (m_dPhi <= M_PI) ? (ds <= 0 && de <= 0) : (ds <= 0 || de <= 0);
}



void GeoCons::exec (GeoShapeAction *action) const {
  action->handleCons(this);
}
