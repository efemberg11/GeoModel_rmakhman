/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoShapeAction.h"

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

GeoCons::~GeoCons()
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

const std::string & GeoCons::type () const
{
  return s_classType;
}

ShapeType GeoCons::typeID () const
{
  return s_classTypeID;
}

void GeoCons::exec (GeoShapeAction *action) const
{
  action->handleCons(this);
}
