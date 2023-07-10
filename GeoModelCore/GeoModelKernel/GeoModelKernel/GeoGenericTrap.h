/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOGENERICTRAP_H
#define GEOMODELKERNEL_GEOGENERICTRAP_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

typedef GeoTrf::Vector2D GeoTwoVector;
typedef std::vector<GeoTwoVector> GeoGenericTrapVertices;

class GeoGenericTrap : public GeoShape
{
 public:
  GeoGenericTrap(double ZHalfLength, const GeoGenericTrapVertices& Vertices);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume() const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns the GENERIC TRAP shape type, as a string.
  virtual const std::string& type() const;

  //    Returns the GENERIC TRAP shape type, as a coded integer.
  virtual ShapeType typeID() const;

  //    For type identification.
  static const std::string& getClassType();

  //    For type identification.
  static ShapeType getClassTypeID();

  //    Executes a GeoShapeAction.
  virtual void exec(GeoShapeAction *action) const;

  //    Z half length.
  double getZHalfLength() const;

  //    Vector of vertices.
  const GeoGenericTrapVertices& getVertices() const;

 protected:
  virtual ~GeoGenericTrap();

 private:
  GeoGenericTrap(const GeoGenericTrap &right);
  GeoGenericTrap& operator=(const GeoGenericTrap &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_zHalfLength;
  GeoGenericTrapVertices m_vertices;
};

inline const std::string& GeoGenericTrap::getClassType()
{
  return s_classType;
}

inline ShapeType GeoGenericTrap::getClassTypeID()
{
  return s_classTypeID;
}

#endif
