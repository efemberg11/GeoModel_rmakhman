/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTWISTEDTRAP_H
#define GEOMODELKERNEL_GEOTWISTEDTRAP_H

/**
 * @class GeoTwistedTrap
 *
 * @brief This shape represents a general twisted trapezoid
 * GeoTwistedTrap
 *
 *Class description:
 *
 * A GeoTwistedTrap is a general twisted trapezoid: The faces perpendicular to the
 * z planes are trapezia, and their centres are not necessarily on
 * a line parallel to the z axis.
 *      pTwist  Phi twist angle
 *      pDz      Half-length along the z-axis
 *      pTheta Polar angle of the line joining the centres of the faces at -/+pDz
 *      pPhi     Azimuthal angle of the line joing the centre of the face at -pDz to the centre of the face at +pDz
 *      pDy1    Half-length along y of the face at -pDz
 *      pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
 *      pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
 *
 *      pDy2    Half-length along y of the face at +pDz
 *      pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
 *      pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
 *      pAlph   Angle with respect to the y axis from the centre of the side
 *
 *
 *  A special regular case of a trapezoid with equal endcaps is available,
 *  with polar,azimuthal and tilt angles set to zero.
*/

#include "GeoModelKernel/GeoShape.h"

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

class GeoTwistedTrap : public GeoShape
{
 public:

  GeoTwistedTrap(double  pPhiTwist,
                 double  pDx1,  // half x length at -pDz,-pDy
                 double  pDx2,  // half x length at -pDz,+pDy
                 double  pDy,
                 double  pDz);

  GeoTwistedTrap(double  pPhiTwist,   // twist angle
                 double  pDz,     // half z length
                 double  pTheta,  // direction between end planes
                 double  pPhi,    // defined by polar and azim. angles
                 double  pDy1,    // half y length at -pDz
                 double  pDx1,    // half x length at -pDz,-pDy
                 double  pDx2,    // half x length at -pDz,+pDy
                 double  pDy2,    // half y length at +pDz
                 double  pDx3,    // half x length at +pDz,-pDy
                 double  pDx4,    // half x length at +pDz,+pDy
                 double  pAlph    // tilt angle
  );

  //    Returns the volume of the shape, for mass inventory
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the TWISTED TRAP shape type, as a string.
  virtual const std::string & type () const {
      return s_classType;
  }

  //    Returns the TWISTED TRAP shape type, as a coded integer.
  virtual ShapeType typeID () const {
     return getClassTypeID();
  }

  //    Returns false as TWISTED TRAP is not a polyhedron.
  virtual bool isPolyhedron () const {
    return false;
  }

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification.
  static const std::string& getClassType () {
     return s_classType;
  }

  //    For type identification.
  static ShapeType getClassTypeID () {
     return s_classTypeID;
  }

  inline double getY1HalfLength() const { return m_dy1 ; }
  inline double getX1HalfLength() const { return m_dx1 ; }
  inline double getX2HalfLength() const { return m_dx2 ; }
  inline double getY2HalfLength() const { return m_dy2 ; }
  inline double getX3HalfLength() const { return m_dx3 ; }
  inline double getX4HalfLength() const { return m_dx4 ; }
  inline double getZHalfLength()  const { return m_dz ; }
  inline double getPhiTwist()     const { return m_phiTwist ; }
  inline double getTheta()        const { return m_theta ; }
  inline double getPhi()          const { return m_phi ; }
  inline double getTiltAngleAlpha() const { return m_alph ; }

 protected:
  virtual ~GeoTwistedTrap() = default;

 private:


  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_theta{0.};
  double m_phi{0.};
  double m_dy1{0.};
  double m_dx1{0.};
  double m_dx2{0.};
  double m_dy2{0.};
  double m_dx3{0.};
  double m_dx4{0.};
  double m_dz{0.};        // Half-length along the z axis
  double m_alph{0.};
  double m_phiTwist{0.};  // twist angle ( dphi in surface equation)

  static constexpr double m_CarTolerance = 1E-9 * SYSTEM_OF_UNITS::mm;
  static constexpr double m_RadTolerance = 1E-9 * SYSTEM_OF_UNITS::mm;
  static constexpr double m_AngTolerance = 1E-9 * SYSTEM_OF_UNITS::rad;

};

#endif
