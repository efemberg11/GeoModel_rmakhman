/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include "PolygonTriangulator.h"//For volume.
#include <cmath>
#include <stdexcept>

const std::string GeoSimplePolygonBrep::s_classType = "SimplePolygonBrep";
const ShapeType GeoSimplePolygonBrep::s_classTypeID = 0x20;

GeoSimplePolygonBrep::GeoSimplePolygonBrep(double dz)
   : m_dZ(dz)
{
}

GeoSimplePolygonBrep::~GeoSimplePolygonBrep()
{
}

double GeoSimplePolygonBrep::volume () const
{
  if (!isValid())
    throw std::runtime_error ("Volume requested for incomplete simple polygon brep");
  int n = getNVertices();
  double area = m_xVertices[n - 1] * m_yVertices[0] - m_xVertices[0] * m_yVertices[n - 1];
  for (int k = 1; k < n; ++k)
  {
    area += m_xVertices[k - 1] * m_yVertices[k] - m_xVertices[k] * m_yVertices[k - 1];
  }
  return std::abs(area) * m_dZ;
}

void GeoSimplePolygonBrep::extent (double& xmin, double& ymin, double& zmin,
                                   double& xmax, double& ymax, double& zmax) const
{
  if (!isValid())
    throw std::runtime_error ("Extent requested for incomplete simple polygon brep");
  xmin = xmax = m_xVertices[0];
  ymin = ymax = m_yVertices[0];
  for (size_t k = 1; k < getNVertices(); ++k)
  {
    double x = m_xVertices[k];
    double y = m_yVertices[k];
    xmin = std::min(xmin, x);
    xmax = std::max(xmax, x);
    ymin = std::min(ymin, y);
    ymax = std::max(ymax, y);
  }
  zmin =-m_dZ;
  zmax = m_dZ;
}

const std::string & GeoSimplePolygonBrep::type() const
{
  return s_classType;
}

ShapeType GeoSimplePolygonBrep::typeID() const
{
  return s_classTypeID;
}

void GeoSimplePolygonBrep::addVertex(double XVertex, double YVertex)
{
  m_xVertices.push_back(XVertex);
  m_yVertices.push_back(YVertex);
}

void GeoSimplePolygonBrep::exec(GeoShapeAction *action) const
{
  action->handleSimplePolygonBrep(this);
}
