/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTwistedTrap.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <cmath>
#include <iostream>

const std::string GeoTwistedTrap::s_classType = "TwistedTrap";
const ShapeType GeoTwistedTrap::s_classTypeID = 0x19; //this code should not be used by other shapes


GeoTwistedTrap::GeoTwistedTrap(double  pPhiTwist,
                               double  pDx1,
                               double  pDx2,
                               double  pDy,
                               double  pDz )
: m_theta(0.),
  m_phi(0.),
  m_dy1(pDy),
  m_dx1(pDx1),
  m_dx2(pDx2),
  m_dy2(pDy),
  m_dx3(pDx1),
  m_dx4(pDx2),
  m_dz(pDz),
  m_alph(0.),
  m_phiTwist(pPhiTwist)
{
}

GeoTwistedTrap::
GeoTwistedTrap(double  pPhiTwist,  // twist angle
               double  pDz,        // half z length
               double  pTheta,  // direction between end planes
               double  pPhi,    // defined by polar and azimuthal angles
               double  pDy1,    // half y length at -pDz
               double  pDx1,    // half x length at -pDz,-pDy
               double  pDx2,    // half x length at -pDz,+pDy
               double  pDy2,    // half y length at +pDz
               double  pDx3,    // half x length at +pDz,-pDy
               double  pDx4,    // half x length at +pDz,+pDy
               double  pAlph )  // tilt angle
: m_theta(pTheta),
  m_phi(pPhi),
  m_dy1(pDy1),
  m_dx1(pDx1),
  m_dx2(pDx2),
  m_dy2(pDy2),
  m_dx3(pDx3),
  m_dx4(pDx4),
  m_dz(pDz),
  m_alph(pAlph),
  m_phiTwist(pPhiTwist)
{
     if  ( ! ( ( m_dx1  > 2*m_CarTolerance)
            && ( m_dx2  > 2*m_CarTolerance)
            && ( m_dx3  > 2*m_CarTolerance)
            && ( m_dx4  > 2*m_CarTolerance)
            && ( m_dy1  > 2*m_CarTolerance)
            && ( m_dy2  > 2*m_CarTolerance)
            && ( m_dz   > 2*m_CarTolerance)
            && ( std::abs(m_phiTwist) > 2*m_AngTolerance )
            && ( std::abs(m_phiTwist) < SYSTEM_OF_UNITS::pi/2 )
            && ( std::abs(m_alph) < SYSTEM_OF_UNITS::pi/2 )
            && ( m_theta < SYSTEM_OF_UNITS::pi/2 && m_theta >= 0 ) )
         )
     {
      std::cout<< "EXCEPTION!!! Invalid dimensions. Too small, or twist angle too big: "<< std::endl
               << " fDx 1-4 = " << m_dx1/SYSTEM_OF_UNITS::cm << ", " << m_dx2/SYSTEM_OF_UNITS::cm << ", "
               << m_dx3/SYSTEM_OF_UNITS::cm << ", " << m_dx4/SYSTEM_OF_UNITS::cm << " cm" << std::endl
               << " fDy 1-2 = " << m_dy1/SYSTEM_OF_UNITS::cm << ", " << m_dy2/SYSTEM_OF_UNITS::cm << ", "
               << " cm" << std::endl
               << " fDz = " << m_dz/SYSTEM_OF_UNITS::cm << " cm" << std::endl
               << " twist angle " << m_phiTwist/SYSTEM_OF_UNITS::deg << " deg"<< std::endl
               << " phi,theta = " << m_phi/SYSTEM_OF_UNITS::deg << ", "  << m_theta/SYSTEM_OF_UNITS::deg << " deg"<< std::endl
               << " tilt angle alpha = " << m_alph/SYSTEM_OF_UNITS::deg << " deg"<< std::endl
               << " twistangle should be > "<< (2*m_AngTolerance)/SYSTEM_OF_UNITS::deg<<" and < "<< (SYSTEM_OF_UNITS::pi/2)/SYSTEM_OF_UNITS::deg << " deg"<<std::endl
               << " theta should be >= 0 and < "<< (SYSTEM_OF_UNITS::pi/2)/SYSTEM_OF_UNITS::deg << " deg"<< std::endl
               << " tilt angle alpha should be < "<< (SYSTEM_OF_UNITS::pi/2)/SYSTEM_OF_UNITS::deg << " deg"<<std::endl
               << std::endl;

     }
}


GeoTwistedTrap::~GeoTwistedTrap()
{
}

double GeoTwistedTrap::volume () const
{
  return m_dz * ((m_dx1 + m_dx2 + m_dx3 + m_dx4) * (m_dy1 + m_dy2) +
                 (m_dx4 + m_dx3 - m_dx2 - m_dx1) * (m_dy2 - m_dy1) * (1./3.));
}

void GeoTwistedTrap::extent (double& xmin, double& ymin, double& zmin,
                             double& xmax, double& ymax, double& zmax) const
{
  double cosPhi = std::cos(m_phi);
  double sinPhi = std::sin(m_phi);
  double tanTheta = std::tan(m_theta);
  double tanAlpha = std::tan(m_alph);

  double xmid1 = m_dy1 * tanAlpha;
  double x1 = std::abs(xmid1 + m_dx1);
  double x2 = std::abs(xmid1 - m_dx1);
  double x3 = std::abs(xmid1 + m_dx2);
  double x4 = std::abs(xmid1 - m_dx2);
  double xmax1 = std::max(std::max(std::max(x1, x2), x3), x4);
  double rmax1 = std::hypot(xmax1, m_dy1);

  double xmid2 = m_dy2 * tanAlpha;
  double x5 = std::abs(xmid2 + m_dx3);
  double x6 = std::abs(xmid2 - m_dx3);
  double x7 = std::abs(xmid2 + m_dx4);
  double x8 = std::abs(xmid2 - m_dx4);
  double xmax2 = std::max(std::max(std::max(x5, x6), x7), x8);
  double rmax2 = hypot(xmax2, m_dy2);

  double x0 = m_dz * tanTheta * cosPhi;
  double y0 = m_dz * tanTheta * sinPhi;
  xmin = std::min(-x0 - rmax1, x0 - rmax2);
  ymin = std::min(-y0 - rmax1, y0 - rmax2);
  xmax = std::max(-x0 + rmax1, x0 + rmax2);
  ymax = std::max(-y0 + rmax1, y0 + rmax2);
  zmin =-m_dz;
  zmax = m_dz;
}

const std::string & GeoTwistedTrap::type () const
{
  return s_classType;
}

ShapeType GeoTwistedTrap::typeID () const
{
  return s_classTypeID;
}

void GeoTwistedTrap::exec (GeoShapeAction* action) const
{
  action->handleTwistedTrap(this);
}
