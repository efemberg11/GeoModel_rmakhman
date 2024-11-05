/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTORUS_H
#define GEOMODELKERNEL_GEOTORUS_H

#include "GeoModelKernel/GeoShape.h"


class GeoTorus : public GeoShape {
 public:
  GeoTorus (double Rmin, double Rmax, double Rtor, double SPhi, double DPhi);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the TORUS shape type, as a string.
  virtual const std::string & type () const {
    return getClassType();
  }

  //    Returns the TORUS shape type, as a coded integer.
  virtual ShapeType typeID () const {
     return getClassTypeID();
  }

  //    Returns false as TORUS is not a polyhedron.
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

  //    Inner radius.
  double  getRMin () const{
     return m_rMin;
  }

  //    Outer radius.
  double  getRMax () const {
     return m_rMax;
  }

  //    Radius of the torus.
  double  getRTor () const {
     return m_rTor;
  }

  //    Starting angle of the segment in radians.
  double  getSPhi () const {
     return m_sPhi;
  }

  //    Delta angle of the segment in radians.
  double getDPhi () const {
     return m_dPhi;
  }

 protected:
  virtual ~GeoTorus() = default;

 private:
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin{0.};
  double m_rMax{0.};
  double m_rTor{0.};
  double m_sPhi{0.};
  double m_dPhi{0.};
};

#endif
