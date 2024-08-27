/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPARA_H
#define GEOMODELKERNEL_GEOPARA_H

#include "GeoModelKernel/GeoShape.h"

class GeoPara : public GeoShape
{
 public:
  //    Constructor for the BOX.
  GeoPara (double XHalfLength, double YHalfLength, double ZHalfLength, double Alpha, double Theta, double Phi);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the PARA shape type, as a string.
  virtual const std::string & type () const {
    return getClassType();
  }

  //    Returns the PARA shape type, as a coded integer.
  virtual ShapeType typeID () const {
    return getClassTypeID();
  }

  //    Returns true as PARA is a polyhedron.
  virtual bool isPolyhedron () const {
    return true;
  }

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification.
  static const std::string& getClassType (){
     return s_classType;
  }

  //    For type identification.
  static ShapeType getClassTypeID () {
    return s_classTypeID;
  }

  //    Half length in the x-direction.
  double  getXHalfLength () const {
     return m_xHalfLength;
  }

  //    Half-length in the y direction.
  double  getYHalfLength () const {
    return m_yHalfLength;
  }

  //    Half-length in the z direction.
  double  getZHalfLength() const {
     return m_zHalfLength;
  }

  //    Polar (theta) angle.
  double  getTheta() const{
     return m_theta;
  }

  //    The angle alpha...between the two sides of the top face
  //    of the parallelapiped.
  double  getAlpha () const {
     return m_alpha;
  }

  //    Azimuthal (phi) angle.
  double  getPhi () const{
     return m_phi;
  }

 protected:
  virtual ~GeoPara() = default;

 private:
 
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_xHalfLength{0.};
  double m_yHalfLength{0.};
  double m_zHalfLength{0.};
  double m_theta{0.};
  double m_alpha{0.};
  double m_phi{0.};
};

#endif
