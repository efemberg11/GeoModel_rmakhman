/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>
#include <stdexcept>

const std::string GeoPgon::s_classType = "Pgon";
const ShapeType GeoPgon::s_classTypeID = 0x14;

GeoPgon::GeoPgon (double SPhi, double DPhi, unsigned int NSides)
  : m_sPhi (SPhi)
  , m_dPhi (DPhi)
  , m_nSides (NSides)
{
}

GeoPgon::~GeoPgon()
{
}

double GeoPgon::volume () const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  if (!isValid ())
    throw std::runtime_error ("Volume requested for incomplete polygon");
  double vol = 0.0;
  for (size_t k = 0; k < getNPlanes() - 1; ++k) {
    double z1 = getZPlane(k);
    double z2 = getZPlane(k + 1);
    double a1 = getRMinPlane(k);
    double a2 = getRMinPlane(k + 1);
    double b1 = getRMaxPlane(k);
    double b2 = getRMaxPlane(k + 1);
    vol += (b1*b1 + b1*b2 + b2*b2 - a1*a1 - a1*a2 - a2*a2) * (z2 - z1);
  }
  int nsides = getNSides();
  double dphi = getDPhi();
  if (dphi > 2.0 * M_PI) dphi = 2.0 * M_PI;
  double alpha = dphi / nsides;
  double sinAlpha = std::sin(alpha);
  double cosHalfAlpha = std::cos(0.5 * alpha);
  return std::abs(vol) * nsides * sinAlpha / (cosHalfAlpha * cosHalfAlpha * 6.0);
}

void GeoPgon::extent (double& xmin, double& ymin, double& zmin,
                      double& xmax, double& ymax, double& zmax) const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  if (!isValid ())
    throw std::runtime_error ("Extent requested for incomplete polygon");
  double rmin = getRMinPlane(0);
  double rmax = getRMaxPlane(0);
  zmin = zmax = getZPlane(0);
  for (size_t k = 1; k < getNPlanes(); ++k)
  {
    rmin = std::min(rmin, getRMinPlane(k));
    rmax = std::max(rmax, getRMaxPlane(k));
    zmin = std::min(zmin, getZPlane(k));
    zmax = std::max(zmax, getZPlane(k));
  }
  size_t nsides = getNSides();
  double dphi = getDPhi();
  if (dphi >  2.0 * M_PI) dphi = 2.0 * M_PI;
  if (dphi == 2.0 * M_PI) rmin = 0.0;
  double alpha = dphi / nsides;
  double invCosHalfAlpha = 1. / std::cos(0.5 * alpha);
  rmin *= invCosHalfAlpha;
  rmax *= invCosHalfAlpha;
  double phi = getSPhi();
  xmin = xmax = rmin * std::cos(phi);
  ymin = ymax = rmin * std::sin(phi);
  for (size_t k = 0; k <= nsides; ++k)
  {
    double x = rmax * std::cos(phi);
    xmin = std::min(xmin, x);
    xmax = std::max(xmax, x);
    double y = rmax * std::sin(phi);
    ymin = std::min(ymin, y);
    ymax = std::max(ymax, y);
    if (rmin > 0.)
    {
      double xx = rmin * std::cos(phi);
      xmin = std::min(xmin, xx);
      xmax = std::max(xmax, xx);
      double yy = rmin * std::sin(phi);
      ymin = std::min(ymin, yy);
      ymax = std::max(ymax, yy);
    }
    phi += alpha;
  }
}

bool GeoPgon::contains (double x, double y, double z) const
{
#ifndef M_PI
  constexpr double M_PI = 3.14159265358979323846;
#endif
  if (!isValid ()) return false;
  size_t nz = getNPlanes();
  if (z < getZPlane(0) || z > getZPlane(nz - 1)) return false;

  constexpr double two_PI = 2.0 * M_PI;
  double sphi = getSPhi();
  double dphi = getDPhi();
  if (dphi < two_PI)
  {
    GeoTrf::Vector2D r(x, y);
    GeoTrf::Vector2D ns(std::sin(sphi), -std::cos(sphi));
    GeoTrf::Vector2D ne(-std::sin(sphi + dphi), std::cos(sphi + dphi));
    double ds = ns.dot(r);
    double de = ne.dot(r);
    bool in_wedge = (dphi <= M_PI) ? (ds <= 0 && de <= 0) : (ds <= 0 || de <= 0);
    if (!in_wedge) return false;
  }

  double r = 0.0;
  if (x * x + y * y > 0.0)
  {
    while (sphi > 0) sphi -= two_PI;
    while (sphi < 0) sphi += two_PI;
    double phi = std::atan2(y, x);
    while (phi < sphi) phi += two_PI;

    if (dphi > two_PI) dphi = two_PI;
    int nsides = getNSides();
    double sector = dphi / nsides;
    int iphi = (phi - sphi) / sector;
    if (iphi == nsides) iphi--;

    double rot = sphi + sector * (iphi + 0.5);
    r = std::cos(rot) * x + std::sin(rot) * y;
  }

  for (size_t k = 0; k < nz - 1; ++k)
  {
    double zmin = getZPlane(k);
    double zmax = getZPlane(k + 1);
    if (z < zmin || z > zmax || zmin == zmax) continue;
    double t = (z - zmin) / (zmax - zmin);
    double rmin1 = getRMinPlane(k);
    double rmin2 = getRMinPlane(k + 1);
    double rmin = rmin1 + (rmin2 - rmin1) * t;
    double rmax1 = getRMaxPlane(k);
    double rmax2 = getRMaxPlane(k + 1);
    double rmax = rmax1 + (rmax2 - rmax1) * t;
    if (r <= rmax && r >= rmin) return true;
  }
  return false;
}

const std::string & GeoPgon::type () const
{
  return s_classType;
}

ShapeType GeoPgon::typeID () const
{
  return s_classTypeID;
}

void GeoPgon::addPlane (double ZPlane, double RMinPlane, double RMaxPlane)
{
  m_zPlane.push_back (ZPlane);
  m_rMinPlane.push_back (RMinPlane);
  m_rMaxPlane.push_back (RMaxPlane);
}

void GeoPgon::exec (GeoShapeAction *action) const
{
  action->handlePgon(this);
}
