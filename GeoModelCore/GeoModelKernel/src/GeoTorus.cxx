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
{
}

GeoTorus::~GeoTorus()
{
}

double GeoTorus::volume () const
{
#ifndef M_PI
  double M_PI = acos (-1.0);
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

const std::string & GeoTorus::type () const
{
  return s_classType;
}

ShapeType GeoTorus::typeID () const
{
  return s_classTypeID;
}

void GeoTorus::exec (GeoShapeAction *action) const
{
  action->handleTorus(this);
}
