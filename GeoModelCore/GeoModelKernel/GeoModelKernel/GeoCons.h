/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCONS_H
#define GEOMODELKERNEL_GEOCONS_H

#include "GeoModelKernel/GeoShape.h"

class GeoCons : public GeoShape {
 public:
  GeoCons (double RMin1, double RMin2, double RMax1, double RMax2, double DZ, double SPhi, double DPhi);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the CONS shape type, as a string.
  virtual const std::string & type () const {
     return getClassType();
  }

  //    Returns the CONS shape type, as a coded integer.
  virtual ShapeType typeID () const {
     return getClassTypeID();
  }

  //    Returns false as CONS is not a polyhedron.
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
  static ShapeType getClassTypeID() {
     return s_classTypeID;
  }

  //    Inside radius at -dZ
  double getRMin1 () const { return m_rMin1; }
  //    Inside radius at +dZ
  double getRMin2 () const { return m_rMin2; }
  //    Outside radius at -dZ
  double getRMax1 () const { return m_rMax1; }
  //    Outside radius at +dZ
  double getRMax2 () const { return m_rMax2; }
  //    Half length in Z direction.
  double getDZ () const { return m_dZ; }
  //    Starting angle of the segment in radians.
  double getSPhi () const { return m_sPhi; }
  //    Delta angle of the segment in radians.
  double getDPhi () const { return m_dPhi; }

protected:
  virtual ~GeoCons() = default;

 private:
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin1{0.};
  double m_rMin2{0.};
  double m_rMax1{0.};
  double m_rMax2{0.};
  double m_dZ{0.};
  double m_sPhi{0.};
  double m_dPhi{0.};
};

#endif
