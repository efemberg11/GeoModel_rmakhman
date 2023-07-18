/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTessellatedSolid.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoTessellatedSolid::s_classType = "TessellatedSolid";
const ShapeType GeoTessellatedSolid::s_classTypeID = 0x21;

GeoTessellatedSolid::GeoTessellatedSolid()
{
}

GeoTessellatedSolid::~GeoTessellatedSolid()
{
  for(size_t i=0; i<m_facets.size(); ++i)
    m_facets[i]->unref();
}

double GeoTessellatedSolid::volume() const
{
  if (!isValid ())
    throw std::runtime_error ("Volume requested for incomplete tessellated solid");
  double v = 0.;
  for (size_t i = 0; i < getNumberOfFacets(); ++i)
  {
    GeoFacet* facet = getFacet(i);
    GeoTrf::Vector3D e1 = facet->getVertex(2) - facet->getVertex(0);
    GeoTrf::Vector3D e2 = (facet->getNumberOfVertices() == 4) ?
      facet->getVertex(3) - facet->getVertex(1) :
      facet->getVertex(2) - facet->getVertex(1);
    v += facet->getVertex(0).dot(e1.cross(e2));
  }
  if (v < 0.)
    throw std::runtime_error ("Incorrect order of vertices in tessellated solid");
  return v*(1./6.);
}

void GeoTessellatedSolid::extent (double& xmin, double& ymin, double& zmin,
                                  double& xmax, double& ymax, double& zmax) const
{
  if (!isValid ())
    throw std::runtime_error ("Extent requested for incomplete tessellated solid");
  GeoFacet* facet = getFacet(0);
  GeoTrf::Vector3D vertex = facet->getVertex(0);
  xmin = xmax = vertex.x();
  ymin = ymax = vertex.y();
  zmin = zmax = vertex.z();
  for (size_t i = 0; i < getNumberOfFacets(); ++i)
  {
    facet = getFacet(i);
    for (size_t k = 0; k < facet->getNumberOfVertices(); ++k)
    {
      vertex = facet->getVertex(k);
      double x = vertex.x();
      double y = vertex.y();
      double z = vertex.z();
      xmin = std::min(xmin, x);
      ymin = std::min(ymin, y);
      zmin = std::min(zmin, z);
      xmax = std::max(xmax, x);
      ymax = std::max(ymax, y);
      zmax = std::max(zmax, z);
    }
  }
}

const std::string& GeoTessellatedSolid::type() const
{
  return s_classType;
}

ShapeType GeoTessellatedSolid::typeID() const
{
  return s_classTypeID;
}

void GeoTessellatedSolid::exec(GeoShapeAction *action) const
{
  action->handleTessellatedSolid(this);
}

void GeoTessellatedSolid::addFacet(GeoFacet* facet)
{
  facet->ref();
  m_facets.push_back(facet);
}

GeoFacet* GeoTessellatedSolid::getFacet(size_t index) const
{
  return (index<m_facets.size() ? m_facets[index] : 0);
}

size_t GeoTessellatedSolid::getNumberOfFacets() const
{
  return m_facets.size();
}
