/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSIMPLEPOLYGONBREP_H
#define GEOMODELKERNEL_GEOSIMPLEPOLYGONBREP_H

/**
 * @class: GeoSimplePolygonBrep
 *
 * @brief This shape represents a BREP solid consisting
 *      of two equivalent polygonial faces perpendicular to Z axis.
 *      The polygones are described by array of (x,y) vertices,
 *      The solid is considered valid if the number of polygon vertices >=3
 *
 *      Constructor parameter is a half length along Z axis
 */

#include <vector>
#include "GeoModelKernel/GeoShape.h"

class GeoSimplePolygonBrep : public GeoShape
{
 public:
  //    Constructor for the BREP.  Note that the constructor
  //    does not fully build this object. The BREP is not valid
  //    until at least three vertices have been added.
  GeoSimplePolygonBrep(double dz);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume() const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the BREP shape type, as a string
  virtual const std::string& type() const;

  //    Returns the BREP shape type, as a coded integer
  virtual ShapeType typeID() const;

  //    For type identification
  static const std::string& getClassType();

  //    For type identification
  static ShapeType getClassTypeID();

  //    Executes a GeoShapeAction
  virtual void exec(GeoShapeAction *action) const;

  //    Add another vertex to the polygon. A minimum of three
  //    vertices are required to create a valid solid.
  void addVertex(double XVertex, double YVertex);

  //    Returns the number of vertices in the polygon
  unsigned int getNVertices() const;

  //    True if the polygon has at least three vertices, false otherwise
  bool isValid () const;

  //    Returns X coordinate of the specified vertex
  const double & getXVertex(unsigned int i) const;

  //    Returns Y coordinate of the specified vertex
  const double & getYVertex(unsigned int i) const;

  //    Half-length along Z axis
  const double& getDZ() const;

 protected:
  virtual ~GeoSimplePolygonBrep();

 private:
  GeoSimplePolygonBrep(const GeoSimplePolygonBrep &right);
  GeoSimplePolygonBrep & operator=(const GeoSimplePolygonBrep &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_dZ;
  std::vector<double> m_xVertices;
  std::vector<double> m_yVertices;
};

inline unsigned int GeoSimplePolygonBrep::getNVertices() const
{
  return m_xVertices.size();
}

inline bool GeoSimplePolygonBrep::isValid() const
{
  return getNVertices()>=3;
}

inline const double& GeoSimplePolygonBrep::getXVertex(unsigned int i) const
{
  return m_xVertices[i];
}

inline const double& GeoSimplePolygonBrep::getYVertex(unsigned int i) const
{
  return m_yVertices[i];
}

inline const std::string& GeoSimplePolygonBrep::getClassType()
{
  return s_classType;
}

inline ShapeType GeoSimplePolygonBrep::getClassTypeID()
{
  return s_classTypeID;
}

inline const double& GeoSimplePolygonBrep::getDZ() const
{
  return m_dZ;
}

#endif
