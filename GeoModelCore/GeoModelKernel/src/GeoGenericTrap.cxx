/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoGenericTrap::s_classType = "GenericTrap";
const ShapeType GeoGenericTrap::s_classTypeID = 0x23;

GeoGenericTrap::GeoGenericTrap(double ZHalfLength, const GeoGenericTrapVertices& Vertices)
  : m_zHalfLength(ZHalfLength)
  , m_vertices(Vertices)
{
}

GeoGenericTrap::~GeoGenericTrap()
{
}

double GeoGenericTrap::volume() const
{
  // diagonals
  GeoTwoVector A = m_vertices[3] - m_vertices[1];
  GeoTwoVector B = m_vertices[2] - m_vertices[0];
  GeoTwoVector C = m_vertices[7] - m_vertices[5];
  GeoTwoVector D = m_vertices[6] - m_vertices[4];

  // kross products
  double AB = A.x()*B.y() - A.y()*B.x();
  double CD = C.x()*D.y() - C.y()*D.x();
  double AD = A.x()*D.y() - A.y()*D.x();
  double CB = C.x()*B.y() - C.y()*B.x();

  return m_zHalfLength * ((AB + CD)* (1./3.) + (AD + CB)* (1./6.));
}

void GeoGenericTrap::extent (double& xmin, double& ymin, double& zmin,
                             double& xmax, double& ymax, double& zmax) const
{
  xmin = xmax = m_vertices[0].x();
  ymin = ymax = m_vertices[0].y();
  int nv = m_vertices.size();
  for (int i = 1; i < nv; ++i)
  {
    if (xmin > m_vertices[i].x()) xmin = m_vertices[i].x();
    if (xmax < m_vertices[i].x()) xmax = m_vertices[i].x();
    if (ymin > m_vertices[i].y()) ymin = m_vertices[i].y();
    if (ymax < m_vertices[i].y()) ymax = m_vertices[i].y();
  }
  zmin =-m_zHalfLength;
  zmax = m_zHalfLength;
}

const std::string& GeoGenericTrap::type() const
{
  return s_classType;
}

ShapeType GeoGenericTrap::typeID() const
{
  return s_classTypeID;
}

void GeoGenericTrap::exec(GeoShapeAction *action) const
{
  action->handleGenericTrap(this);
}

double GeoGenericTrap::getZHalfLength() const
{
  return m_zHalfLength;
}

const GeoGenericTrapVertices& GeoGenericTrap::getVertices() const
{
  return m_vertices;
}
