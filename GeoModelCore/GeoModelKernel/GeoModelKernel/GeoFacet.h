/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOFACET_H
#define GEOMODELKERNEL_GEOFACET_H

/**
 * @class: GeoFacet
 *
 * @brief Base class for GeoModel Facets
 * Two implementations exist:
 *   1. GeoTriangularFacet     
 *   2. GeoQuadrangularFacet
 */

#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

using GeoFacetVertex = GeoTrf::Vector3D;

// ** Base class
class GeoFacet : public RCBase
{
 public:
  enum GeoFacetVertexType {
    ABSOLUTE,
    RELATIVE
  };

  inline size_t getNumberOfVertices() const {
     return m_nVertices;
  }
  inline GeoFacetVertex getVertex(size_t index) const {
    return (index<m_nVertices ? m_vertices[index] : GeoFacetVertex(999999.,999999.,999999.));
  }
  inline GeoFacetVertexType getVertexType() const {
     return m_vertexType;
  }

 protected:
  GeoFacet() = default;
  virtual ~GeoFacet() = default;

  size_t m_nVertices{0};
  std::vector<GeoFacetVertex> m_vertices{};
  GeoFacetVertexType m_vertexType{GeoFacetVertexType::ABSOLUTE};
};

// Triangular facet
class GeoTriangularFacet : public GeoFacet
{
 public:
  GeoTriangularFacet(GeoFacetVertex
             ,GeoFacetVertex
             ,GeoFacetVertex
             ,GeoFacetVertexType);

  virtual ~GeoTriangularFacet() = default;
};

// Quadrangular facet
class GeoQuadrangularFacet : public GeoFacet {
 public:
  GeoQuadrangularFacet(GeoFacetVertex ,GeoFacetVertex, GeoFacetVertex,
                       GeoFacetVertex ,GeoFacetVertexType);

  virtual ~GeoQuadrangularFacet() = default;
};


#endif
