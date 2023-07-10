/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoTrap::s_classType = "Trap";
const ShapeType GeoTrap::s_classTypeID = 0x15;

GeoTrap::GeoTrap (double ZHalfLength, double Theta, double Phi, double Dydzn, double Dxdyndzn, double Dxdypdzn, double Angleydzn, double Dydzp, double Dxdyndzp, double Dxdypdzp, double Angleydzp)
  : m_zHalfLength (ZHalfLength)
  , m_theta (Theta)
  , m_phi (Phi)
  , m_dydzn (Dydzn)
  , m_dxdyndzn (Dxdyndzn)
  , m_dxdypdzn (Dxdypdzn)
  , m_angleydzn (Angleydzn)
  , m_dydzp (Dydzp)
  , m_dxdyndzp (Dxdyndzp)
  , m_dxdypdzp (Dxdypdzp)
  , m_angleydzp (Angleydzp)
{
}

GeoTrap::~GeoTrap()
{
}

double GeoTrap::volume () const
{
  double dz = m_zHalfLength;
  double dy1 = m_dydzn;
  double dx1 = m_dxdyndzn;
  double dx2 = m_dxdypdzn;
  double dy2 = m_dydzp;
  double dx3 = m_dxdyndzp;
  double dx4 = m_dxdypdzp;
  return dz * ((dx1 + dx2 + dx3 + dx4) * (dy1 + dy2) +
               (dx4 + dx3 - dx2 - dx1) * (dy2 - dy1) * (1./3.));
}

void GeoTrap::extent (double& xmin, double& ymin, double& zmin,
                      double& xmax, double& ymax, double& zmax) const
{
  double dz = m_zHalfLength;
  double dy1 = m_dydzn;
  double dx1 = m_dxdyndzn;
  double dx2 = m_dxdypdzn;
  double dy2 = m_dydzp;
  double dx3 = m_dxdyndzp;
  double dx4 = m_dxdypdzp;
  double theta = m_theta;
  double phi = m_phi;
  double alpha1 = m_angleydzn;
  double alpha2 = m_angleydzp;

  double dzTthetaCphi = dz * std::tan(theta) * std::cos(phi);
  double dzTthetaSphi = dz * std::tan(theta) * std::sin(phi);
  double dy1Talpha1 = dy1 * std::tan(alpha1);
  double dy2Talpha2 = dy2 * std::tan(alpha2);

  double x1 =-dzTthetaCphi - dy1Talpha1 - dx1;
  double x2 =-dzTthetaCphi + dy1Talpha1 - dx2;
  double x3 = dzTthetaCphi - dy2Talpha2 - dx3;
  double x4 = dzTthetaCphi + dy2Talpha2 - dx4;
  double x5 =-dzTthetaCphi - dy1Talpha1 + dx1;
  double x6 =-dzTthetaCphi + dy1Talpha1 + dx2;
  double x7 = dzTthetaCphi - dy2Talpha2 + dx3;
  double x8 = dzTthetaCphi + dy2Talpha2 + dx4;

  double y1 =-dzTthetaSphi - dy1;
  double y2 = dzTthetaSphi - dy2;
  double y3 =-dzTthetaSphi + dy1;
  double y4 = dzTthetaSphi + dy2;

  xmin = std::min(std::min(std::min(x1, x2), x3), x4);
  xmax = std::max(std::max(std::max(x5, x6), x7), x8);
  ymin = std::min(y1, y2);
  ymax = std::max(y3, y4);
  zmin =-dz;
  zmax = dz;
}

const std::string & GeoTrap::type () const
{
  return s_classType;
}

ShapeType GeoTrap::typeID () const
{
  return s_classTypeID;
}

void GeoTrap::exec (GeoShapeAction *action) const
{
  action->handleTrap(this);
}
