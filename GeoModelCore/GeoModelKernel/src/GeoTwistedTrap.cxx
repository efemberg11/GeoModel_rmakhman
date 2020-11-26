/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTwistedTrap.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoTwistedTrap::s_classType = "TwistedTrap";
const ShapeType GeoTwistedTrap::s_classTypeID = 0x19; //this code should not be used by other shapes
 

GeoTwistedTrap::GeoTwistedTrap(double  pPhiTwist,
                             double  pDx1,
                             double  pDx2,
                             double  pDy,
                             double  pDz )
: m_phiTwist(pPhiTwist),
m_dz(pDz),
m_theta(0.),
m_phi(0.),
m_dy1(pDy),
m_dx1(pDx1),
m_dx2(pDx2),
m_dy2(pDy),
m_dx3(pDx1),
m_dx4(pDx2),
m_alph(0.)
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
: m_phiTwist(pPhiTwist),
m_dz(pDz),
m_theta(pTheta),
m_phi(pPhi),
m_dy1(pDy1),
m_dx1(pDx1),
m_dx2(pDx2),
m_dy2(pDy2),
m_dx3(pDx3),
m_dx4(pDx4),
m_alph(pAlph)
{
}


GeoTwistedTrap::~GeoTwistedTrap()
{
}

double GeoTwistedTrap::volume () const
{
  //not really
  double cubicVolume = 2 * m_dz * ( ( m_dx1 + m_dx2 ) * m_dy1 + ( m_dx3 + m_dx4 ) * m_dy2  );
  return cubicVolume;
}

const std::string & GeoTwistedTrap::type () const
{
  return s_classType;
}

ShapeType GeoTwistedTrap::typeID () const
{
  return s_classTypeID;
}

void GeoTwistedTrap::exec (GeoShapeAction *action) const
{
  action->handleTwistedTrap(this); 
}
