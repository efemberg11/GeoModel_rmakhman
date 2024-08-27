/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoGenericTrap::s_classType = "GenericTrap";
const ShapeType GeoGenericTrap::s_classTypeID = 0x23;

GeoGenericTrap::GeoGenericTrap(double ZHalfLength, const GeoGenericTrapVertices& Vertices)
  : m_zHalfLength(ZHalfLength)
  , m_vertices(Vertices)
{}

double GeoGenericTrap::volume(int) const
{
  // diagonals
  GeoTwoVector a = m_vertices[3] - m_vertices[1];
  GeoTwoVector b = m_vertices[2] - m_vertices[0];
  GeoTwoVector c = m_vertices[7] - m_vertices[5];
  GeoTwoVector d = m_vertices[6] - m_vertices[4];

  // kross products
  double ab = a.x()*b.y() - a.y()*b.x();
  double cd = c.x()*d.y() - c.y()*d.x();
  double ad = a.x()*d.y() - a.y()*d.x();
  double cb = c.x()*b.y() - c.y()*b.x();

  return m_zHalfLength * ((ab + cd)* (1./3.) + (ad + cb)* (1./6.));
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

bool GeoGenericTrap::contains (double x, double y, double z) const
{
  if (std::abs(z) - m_zHalfLength > 0.0) return false;
  double t = 0.5 * (1.0 + z / m_zHalfLength);
  GeoTrf::Vector2D v[4];
  v[0] = m_vertices[0] + (m_vertices[4] - m_vertices[0]) * t;
  v[1] = m_vertices[1] + (m_vertices[5] - m_vertices[1]) * t;
  v[2] = m_vertices[2] + (m_vertices[6] - m_vertices[2]) * t;
  v[3] = m_vertices[3] + (m_vertices[7] - m_vertices[3]) * t;
  int nv = 4;
  bool in = false;
  for (int i = 0, k = nv - 1; i < nv; k = i++)
  {
    if ((v[i].y() > y) != (v[k].y() > y))
    {
      double ctg = (v[k].x() - v[i].x()) / (v[k].y() - v[i].y());
      in ^= (x < (y - v[i].y()) * ctg + v[i].x());
    }
  }
  return in;
}

void GeoGenericTrap::exec(GeoShapeAction *action) const
{
  action->handleGenericTrap(this);
}

