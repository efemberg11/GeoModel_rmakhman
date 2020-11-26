/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTWISTEDTRAP_H
#define GEOMODELKERNEL_GEOTWISTEDTRAP_H

/**
 * @class GeoTwistedTrap
 *
 * @brief This shape represents a general twisted trapezoid
 * G4TwistedTrap
 *
 *Class description:
 *
 * A G4TwistedTrap is a general twisted trapezoid: The faces perpendicular to the
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

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the TWISTED TRAP shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the TWISTED TRAP shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
    
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
  inline double getTiltAngleAlpha() const { return m_tAlph ; }
  
 protected:
  virtual ~GeoTwistedTrap();

 private:
  GeoTwistedTrap(const GeoTwistedTrap &right);
  GeoTwistedTrap & operator=(const GeoTwistedTrap &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_theta;
  double m_phi ;
  double m_dy1;
  double m_dx1;
  double m_dx2;
  double m_dy2;
  double m_dx3;
  double m_dx4;
  double m_dz;        // Half-length along the z axis
  double m_dx ;       // maximum side in x
  double m_dy ;       // maximum side in y
  double m_alph ;
  double m_tAlph ;    // std::tan(fAlph)
  double m_deltaX ;
  double m_deltaY ;
  double m_phiTwist;  // twist angle ( dphi in surface equation)

};

inline const std::string& GeoTwistedTrap::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoTwistedTrap::getClassTypeID ()
{
  return s_classTypeID;
}

//inline const double& GeoTwistedTrap::getZHalfLength () const
//{
//  return m_zHalfLength;
//}
//
//inline const double& GeoTwistedTrap::getTheta () const
//{
//  return m_theta;
//}
//
//inline const double& GeoTwistedTrap::getPhi () const
//{
//  return m_phi;
//}
//
//inline const double& GeoTwistedTrap::getDydzn () const
//{
//  return m_dydzn;
//}
//
//inline const double& GeoTwistedTrap::getDxdyndzn () const
//{
//  return m_dxdyndzn;
//}
//inline const double& GeoTwistedTrap::getDxdypdzn () const
//{
//  return m_dxdypdzn;
//}
//
//inline const double& GeoTwistedTrap::getAngleydzn () const
//{
//  return m_angleydzn;
//}
//
//inline const double& GeoTwistedTrap::getDydzp () const
//{
//  return m_dydzp;
//}
//
//inline const double& GeoTwistedTrap::getDxdyndzp () const
//{
//  return m_dxdyndzp;
//}
//
//inline const double& GeoTwistedTrap::getDxdypdzp () const
//{
//  return m_dxdypdzp;
//}
//
//inline const double& GeoTwistedTrap::getAngleydzp () const
//{
//  return m_angleydzp;
//}

#endif
