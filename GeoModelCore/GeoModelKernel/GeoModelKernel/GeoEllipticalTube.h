/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOELLIPTICALTUBE_H
#define GEOMODELKERNEL_GEOELLIPTICALTUBE_H

/**
 * @class: GeoEllipticalTube
 *
 * @brief This is a tube with elliptical cross section
 * The equation of the surface in x/y is 1.0 = (x/dx)**2 + (y/dy)**2
 */

#include "GeoModelKernel/GeoShape.h"

class GeoEllipticalTube : public GeoShape {
 public:
  //    Constructor for the ELLIPTICAL TUBE
  GeoEllipticalTube(double XHalfLength, double YHalfLength, double ZHalfLength);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the ELLIPTICAL TUBE shape type, as a string
  virtual const std::string & type () const{
     return getClassType();
  }

  //    Returns the ELLIPTICAL TUBE shape type, as a coded integer
  virtual ShapeType typeID () const {
    return getClassTypeID();
  }

  //    Returns false as ELLIPTICAL TUBE is not a polyhedron.
  virtual bool isPolyhedron () const {
    return false;
  }

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification
  static const std::string& getClassType() {
     return s_classType;
  }

  //    For type identification
  static ShapeType getClassTypeID () {
    return s_classTypeID;
  }

  //    X semi-axis
  double getXHalfLength() const {
    return m_xHalfLength;
  }

  //    Y semi-axis
  double getYHalfLength() const {
    return m_yHalfLength;
  }

  //    Elliptical tube half-length in the Z direction
  double getZHalfLength() const {
    return m_zHalfLength;
  }

 protected:
  virtual ~GeoEllipticalTube() = default;

 private:
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_xHalfLength{0.};
  double m_yHalfLength{0.};
  double m_zHalfLength{0.};
};

#endif
