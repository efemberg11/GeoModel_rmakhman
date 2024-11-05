/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTESSELLATEDSOLID_H
#define GEOMODELKERNEL_GEOTESSELLATEDSOLID_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoFacet.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"

class GeoTessellatedSolid : public GeoShape {
 public:
  //    Constructor for the TESSELLATED SOLID.  Note that the constructor
  //    does not fully build this object.  The TESSELLATED SOLID is not valid
  //    until at least four facets have been added.
  GeoTessellatedSolid();

  //    Returns the volume of the shape, for mass inventory
  virtual double volume(int npoints = 0) const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the TESSELLATED SOLID shape type, as a string
  virtual const std::string& type() const{
     return getClassType();
  }

  //    Returns the TESSELLATED SOLID shape type, as a coded integer
  virtual ShapeType typeID() const {
     return getClassTypeID();
  }

  //    Returns true as TESSELLATED SOLID is a polyhedron.
  virtual bool isPolyhedron () const {
    return true;
  }

  //    For type identification
  static const std::string& getClassType();

  //    For type identification
  static ShapeType getClassTypeID();

  //    Executes a GeoShapeAction
  virtual void exec(GeoShapeAction* action) const;

  //    Add another facet to the tessellated solid. A minimum of four
  //    facets are required to create a valid tesselated solid.
  void addFacet(GeoFacet* facet);

  //    Returns specified facet
  GeoFacet* getFacet(size_t index) const;

  //    Returns the number of facets
  size_t getNumberOfFacets() const;

  // True if the tessellated solid has at least four facets (tetrahedron),
  // false otherwise.
  bool isValid () const;

 protected:
  virtual ~GeoTessellatedSolid() = default;

 private:
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  std::vector<GeoIntrusivePtr<GeoFacet>> m_facets;
};

inline const std::string& GeoTessellatedSolid::getClassType()
{
  return s_classType;
}

inline ShapeType GeoTessellatedSolid::getClassTypeID()
{
  return s_classTypeID;
}

inline bool GeoTessellatedSolid::isValid () const
{
  return m_facets.size () >= 4;
}

#endif
