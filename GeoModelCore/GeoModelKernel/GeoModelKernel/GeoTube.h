/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTUBE_H
#define GEOMODELKERNEL_GEOTUBE_H

#include "GeoModelKernel/GeoShape.h"

class GeoTube : public GeoShape
{
 public:
  GeoTube (double RMin, double RMax, double ZHalfLength);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the TUBE shape type, as a string.
  virtual const std::string & type () const{
     return getClassType();
  }

  //    Returns the TUBE shape type, as a coded integer.
  virtual ShapeType typeID () const{
     return getClassTypeID();
  }

  //    Returns false as TUBE is not a polyhedron.
  virtual bool isPolyhedron () const {
    return false;
  }

  //    Executes a GeoShapeAction.
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification.
  static const std::string& getClassType () {
     return s_classType;
  }

  //    For type identification.
  static ShapeType getClassTypeID () {
    return s_classTypeID;
  }

  //    Minimum (inner) tube radius.
  double  getRMin () const {
     return m_rMin;
  }

  //    Maximum (outer) tube radius.
  double  getRMax () const {
     return m_rMax;
  }

  //    Tube half-length in the z direction.
  double  getZHalfLength () const {
    return m_zHalfLength;
  }

 protected:
  virtual ~GeoTube() = default;

 private:
  GeoTube(const GeoTube &right);
  GeoTube & operator=(const GeoTube &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin{0.};
  double m_rMax{0.};
  double m_zHalfLength{0.};

};

#endif
