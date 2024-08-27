/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOGENERICTRAP_H
#define GEOMODELKERNEL_GEOGENERICTRAP_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

using GeoTwoVector = GeoTrf::Vector2D ;
using GeoGenericTrapVertices = std::vector<GeoTwoVector>;

class GeoGenericTrap : public GeoShape {
 public:
  GeoGenericTrap(double ZHalfLength, const GeoGenericTrapVertices& Vertices);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume(int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the GENERIC TRAP shape type, as a string.
  virtual const std::string& type() const{
     return getClassType();
  }

  //    Returns the GENERIC TRAP shape type, as a coded integer.
  virtual ShapeType typeID() const{
     return getClassTypeID();
  }

  //    Returns false as GENERIC TRAP is not a polyhedron.
  virtual bool isPolyhedron () const {
    return false;
  }

  //    For type identification.
  static const std::string& getClassType() {
    return s_classType;
  }

  //    For type identification.
  static ShapeType getClassTypeID() {
     return s_classTypeID;
  }

  //    Executes a GeoShapeAction.
  virtual void exec(GeoShapeAction *action) const;

  //    Z half length.
  double getZHalfLength() const{
     return m_zHalfLength;
  }

  //    Vector of vertices.
  const GeoGenericTrapVertices& getVertices() const {
    return m_vertices;
  }

 protected:
  virtual ~GeoGenericTrap() = default;

 private:

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_zHalfLength{0};
  GeoGenericTrapVertices m_vertices{};
};

#endif
