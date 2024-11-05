/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoTrd::s_classType = "Trd";
const ShapeType GeoTrd::s_classTypeID = 0x16;

GeoTrd::GeoTrd (double XHalfLength1, double XHalfLength2, double YHalfLength1, double YHalfLength2, double ZHalfLength)
  : m_xHalfLength1 (XHalfLength1)
  , m_xHalfLength2 (XHalfLength2)
  , m_yHalfLength1 (YHalfLength1)
  , m_yHalfLength2 (YHalfLength2)
  , m_zHalfLength (ZHalfLength)
{}

double GeoTrd::volume (int) const
{
  double fDz = m_zHalfLength;
  double fDy1 = m_yHalfLength1;
  double fDx1 = m_xHalfLength1;
  double fDx2 = m_xHalfLength2;
  double fDy2 = m_yHalfLength2;
  return 2.0 * fDz *
    ((fDx1 + fDx2) * (fDy1 + fDy2) + (fDx2 - fDx1) * (fDy2 - fDy1) * (1./3.));
}

void GeoTrd::extent (double& xmin, double& ymin, double& zmin,
                     double& xmax, double& ymax, double& zmax) const
{
  xmax = std::max(m_xHalfLength1, m_xHalfLength2);
  ymax = std::max(m_yHalfLength1, m_yHalfLength2);
  zmax = m_zHalfLength;
  xmin = -xmax;
  ymin = -ymax;
  zmin = -zmax;
}

bool GeoTrd::contains (double x, double y, double z) const
{
  if (std::abs(z) - m_zHalfLength > 0.0) return false;
  double t = 0.5 * (1.0 + z / m_zHalfLength);
  double dx = m_xHalfLength1 + (m_xHalfLength2 - m_xHalfLength1) * t;
  double dy = m_yHalfLength1 + (m_yHalfLength2 - m_yHalfLength1) * t;
  double distx = std::abs(x) - dx;
  double disty = std::abs(y) - dy;
  return (std::max(distx, disty) <= 0.0);
}



void GeoTrd::exec (GeoShapeAction *action) const
{
  action->handleTrd(this);
}
