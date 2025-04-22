/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOBOX_H
#define GEOMODELKERNEL_GEOBOX_H

#include "GeoModelKernel/GeoShape.h"

class GeoBox : public GeoShape
{
 public:
  //    Constructor for the BOX
  GeoBox (double XHalfLength, double YHalfLength, double ZHalfLength);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the BOX shape type, as a string
  virtual const std::string & type () const {
     return getClassType();
  }

  //    Returns the BOX shape type, as a coded integer
  virtual ShapeType typeID() const {
     return getClassTypeID();
  }

  //    Returns true as BOX is a polyhedron.
  virtual bool isPolyhedron () const {
    return true;
  }

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification
  static const std::string& getClassType () {
     return s_classType;
  }

  //    For type identification
  static ShapeType getClassTypeID () {
     return s_classTypeID;
  }

  //    Half length in the x-direction
  double getXHalfLength () const {
      return m_xHalfLength;
  }

  //    Half-length in the y direction
  double getYHalfLength () const {
    return m_yHalfLength;
  }

  //    Half-length in the z direction
  double getZHalfLength () const {
     return m_zHalfLength;
  }

 protected:
  virtual ~GeoBox() = default;

  static const std::string s_classType;
  static const ShapeType s_classTypeID;
  double m_xHalfLength{0.};
  double m_yHalfLength{0.};
  double m_zHalfLength{0.};
};


#endif
