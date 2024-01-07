/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTRAP_H
#define GEOMODELKERNEL_GEOTRAP_H

/**
 * @class GeoTrap
 *
 * @brief This shape represents a very general trapezoid
 */

#include "GeoModelKernel/GeoShape.h"

class GeoTrap : public GeoShape
{
 public:
  GeoTrap (double ZHalfLength, double Theta, double Phi, double Dydzn, double Dxdyndzn, double Dxdypdzn, double Angleydzn, double Dydzp, double Dxdyndzp, double Dxdypdzp, double Angleydzp);

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume () const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the TRAP shape type, as a string.
  virtual const std::string & type () const {
     return getClassType();
  }

  //    Returns the TRAP shape type, as a coded integer.
  virtual ShapeType typeID() const {
     return getClassTypeID();
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

  //    Z half length.
  double getZHalfLength () const { return m_zHalfLength; }

  //    Polar (theta) angle.
  double getTheta () const { return m_theta; }
  //    Azimuthal (phi) angle.
  double getPhi () const { return m_phi; }

  //    Y half length at -z.
  double getDydzn () const { return m_dydzn; }

  //    X half length at -z, -y.
  double getDxdyndzn () const { return m_dxdyndzn; }

  //    X half length at -z, +y.
  double getDxdypdzn () const { return m_dxdypdzn; }
  
  double getAngleydzn () const { return m_angleydzn; }

  //    Y half length at +z.
  double getDydzp () const { return m_dydzp; }

  //    X half length at +z, -y.
  double getDxdyndzp () const { return m_dxdyndzp; }

  //    X half length at +z, +y.
  double getDxdypdzp () const { return m_dxdypdzp; }
  
  double getAngleydzp () const { return m_angleydzp; }

 protected:
  virtual ~GeoTrap() = default;

 private:


  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_zHalfLength{0.};
  double m_theta{0.};
  double m_phi{0.};
  double m_dydzn{0.};
  double m_dxdyndzn{0.};
  double m_dxdypdzn{0.};
  double m_angleydzn{0.};
  double m_dydzp{0.};
  double m_dxdyndzp{0.};
  double m_dxdypdzp{0.};
  double m_angleydzp{0.};

};


#endif
