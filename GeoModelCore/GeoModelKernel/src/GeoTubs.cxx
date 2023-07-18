/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoShapeAction.h"

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

GeoTubs::~GeoTubs()
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

const std::string & GeoTubs::type () const
{
  return s_classType;
}

ShapeType GeoTubs::typeID () const
{
  return s_classTypeID;
}

void GeoTubs::exec (GeoShapeAction *action) const
{
  action->handleTubs(this);
}
