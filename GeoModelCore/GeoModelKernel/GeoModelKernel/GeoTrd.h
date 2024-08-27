/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTRD_H
#define GEOMODELKERNEL_GEOTRD_H

#include "GeoModelKernel/GeoShape.h"

class GeoTrd : public GeoShape
{
 public:
  GeoTrd (double XHalfLength1, double XHalfLength2, double YHalfLength1, double YHalfLength2, double ZHalfLength);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the TRD shape type, as a string.
  virtual const std::string & type () const{
     return getClassType();
  }

  //    Returns the TRD shape type, as a coded integer.
  virtual ShapeType typeID () const {
     return getClassTypeID();
  }

  //    Returns true as TRD is a polyhedron.
  virtual bool isPolyhedron () const {
    return true;
  }

  //    Executes a GeoShapeAction.
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification.
  static const std::string& getClassType() {
     return s_classType;
  }

  //    For type identification.
  static ShapeType getClassTypeID(){
     return s_classTypeID;
  }

  //    Half length in the x-direction at -dz.
  double  getXHalfLength1 () const { return m_xHalfLength1; }

  //    Half length in the x-direction at +dz.
  double  getXHalfLength2 () const { return m_xHalfLength2; }

  //    Half-length in the y direction at -dz.
  double  getYHalfLength1 () const { return m_yHalfLength1; } 

  //    Half-length in the y direction at +dz.
  double  getYHalfLength2 () const { return m_yHalfLength2; } 

  //    Half-length in the z direction.
  double  getZHalfLength () const { return m_zHalfLength; } 

 protected:
  virtual ~GeoTrd() = default;

 private:

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_xHalfLength1{0.};
  double m_xHalfLength2{0.};
  double m_yHalfLength1{0.};
  double m_yHalfLength2{0.};
  double m_zHalfLength{0.};

};

#endif
