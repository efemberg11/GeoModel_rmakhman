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

class GeoEllipticalTube : public GeoShape
{
 public:
  //    Constructor for the ELLIPTICAL TUBE
  GeoEllipticalTube(double XHalfLength, double YHalfLength, double ZHalfLength);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns the ELLIPTICAL TUBE shape type, as a string
  virtual const std::string & type () const;

  //    Returns the ELLIPTICAL TUBE shape type, as a coded integer
  virtual ShapeType typeID () const;

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification
  static const std::string& getClassType ();

  //    For type identification
  static ShapeType getClassTypeID ();

  //    X semi-axis
  const double& getXHalfLength() const;

  //    Y semi-axis
  const double& getYHalfLength() const;

  //    Elliptical tube half-length in the Z direction
  const double& getZHalfLength() const;

 protected:
  virtual ~GeoEllipticalTube();

 private:

  GeoEllipticalTube(const GeoEllipticalTube &right);
  GeoEllipticalTube & operator=(const GeoEllipticalTube &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_xHalfLength;
  double m_yHalfLength;
  double m_zHalfLength;
};

inline const std::string& GeoEllipticalTube::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoEllipticalTube::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoEllipticalTube::getXHalfLength() const
{
  return m_xHalfLength;
}

inline const double& GeoEllipticalTube::getYHalfLength() const
{
  return m_yHalfLength;
}

inline const double& GeoEllipticalTube::getZHalfLength() const
{
  return m_zHalfLength;
}

#endif
