/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include <cmath>
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include "GeoModelKernel/GeoPara.h"

const std::string GeoPara::s_classType = "Para";
const ShapeType GeoPara::s_classTypeID = 0x12;
GeoPara::GeoPara (double XHalfLength, double YHalfLength, double ZHalfLength, double Alpha, double Theta, double Phi)
  :
m_xHalfLength (XHalfLength),
m_yHalfLength (YHalfLength),
m_zHalfLength (ZHalfLength),
m_theta (Theta),
m_alpha (Alpha),
m_phi (Phi)
{}

double GeoPara::volume (int) const
{
  return 8.0 * m_xHalfLength * m_yHalfLength * m_zHalfLength;
}

void GeoPara::extent (double& xmin, double& ymin, double& zmin,
                      double& xmax, double& ymax, double& zmax) const
{
  double dx = m_xHalfLength;
  double dy = m_yHalfLength;
  double dz = m_zHalfLength;

  double x0 = dz * std::tan(m_theta) * std::cos(m_phi);
  double x1 = dy * std::tan(m_alpha);
  xmin = std::min(std::min(std::min(-x0-x1-dx, -x0+x1-dx), x0-x1-dx), x0+x1-dx);
  xmax = std::max(std::max(std::max(-x0-x1+dx, -x0+x1+dx), x0-x1+dx), x0+x1+dx);

  double y0 = dz * std::tan(m_theta) * std::sin(m_phi);
  ymin = std::min(-y0-dy, y0-dy);
  ymax = std::max(-y0+dy, y0+dy);

  zmin = -dz;
  zmax = dz;
}

bool GeoPara::contains (double x, double y, double z) const
{
  double cosPhi = std::cos(m_phi);
  double sinPhi = std::sin(m_phi);
  double tanTheta = std::tan(m_theta);
  double tanAlpha = std::tan(m_alpha);

  double z0 = z;
  double y0 = y - z0 * tanTheta * sinPhi;
  double x0 = x - y0 * tanAlpha - z0 * tanTheta * cosPhi;
  double distx = std::abs(x0) - m_xHalfLength;
  double disty = std::abs(y0) - m_yHalfLength;
  double distz = std::abs(z0) - m_zHalfLength;
  return (std::max(std::max(distx, disty), distz) <= 0.0);
}

void GeoPara::exec (GeoShapeAction *action) const
{
  action->handlePara(this);
}
