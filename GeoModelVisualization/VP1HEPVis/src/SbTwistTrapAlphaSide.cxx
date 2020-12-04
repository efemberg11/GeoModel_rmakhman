//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// G4TwistTrapAlphaSide implementation
//
// Author: 18/03/2005 - O.Link (Oliver.Link@cern.ch)
// --------------------------------------------------------------------

#include <cmath>

#include "VP1HEPVis/SbTwistTrapAlphaSide.h"
//#include "G4PhysicalConstants.hh"
//#include "G4JTPolynomialSolver.hh"

//=====================================================================
//* constructors ------------------------------------------------------

SbTwistTrapAlphaSide::
SbTwistTrapAlphaSide(const std::string& name,
                     double      PhiTwist,  // twist angle
                     double      pDz,       // half z lenght
                     double      pTheta,    // direction between end planes
                     double      pPhi,      // by polar and azimutal angles
                     double      pDy1,      // half y length at -pDz
                     double      pDx1,      // half x length at -pDz,-pDy
                     double      pDx2,      // half x length at -pDz,+pDy
                     double      pDy2,      // half y length at +pDz
                     double      pDx3,      // half x length at +pDz,-pDy
                     double      pDx4,      // half x length at +pDz,+pDy
                     double      pAlph,     // tilt angle at +pDz
                     double      AngleSide  // parity
                                             )
  : SbTwistSurface(name)
{
//  fAxis[0]    = kYAxis; // in local coordinate system
//  fAxis[1]    = kZAxis;
//  fAxisMin[0] = -kInfinity ;  // Y Axis boundary
//  fAxisMax[0] = kInfinity ;   //   depends on z !!
//  fAxisMin[1] = -pDz ;      // Z Axis boundary
//  fAxisMax[1] = pDz ;
  
  fDx1  = pDx1 ;
  fDx2  = pDx2 ;
  fDx3  = pDx3 ;
  fDx4  = pDx4 ;

  fDy1   = pDy1 ;
  fDy2   = pDy2 ;

  fDz   = pDz ;

  fAlph = pAlph  ;
  fTAlph = std::tan(fAlph) ;

  fTheta = pTheta ;
  fPhi   = pPhi ;

  // precalculate frequently used parameters
  fDx4plus2  = fDx4 + fDx2 ;
  fDx4minus2 = fDx4 - fDx2 ;
  fDx3plus1  = fDx3 + fDx1 ;
  fDx3minus1 = fDx3 - fDx1 ;
  fDy2plus1  = fDy2 + fDy1 ;
  fDy2minus1 = fDy2 - fDy1 ;

  fa1md1 = 2*fDx2 - 2*fDx1  ;
  fa2md2 = 2*fDx4 - 2*fDx3 ;

  fPhiTwist = PhiTwist ;     // dphi
  fAngleSide = AngleSide ;  // 0,90,180,270 deg

//  fdeltaX = 2 * fDz * std::tan(fTheta) * std::cos(fPhi);
//    // dx in surface equation
//  fdeltaY = 2 * fDz * std::tan(fTheta) * std::sin(fPhi);
//    // dy in surface equation
//
//  fRot.rotateZ( AngleSide ) ;
//
//  fTrans.set(0, 0, 0);  // No Translation
//  fIsValidNorm = false;
  
  //SetCorners() ;
  //SetBoundaries() ;
}


//=====================================================================
//* Fake default constructor ------------------------------------------
//
//SbTwistTrapAlphaSide::SbTwistTrapAlphaSide()
//  : G4VTwistSurface(a), fTheta(0.), fPhi(0.), fDy1(0.), fDx1(0.), fDx2(0.),
//    fDy2(0.), fDx3(0.), fDx4(0.), fDz(0.), fAlph(0.), fTAlph(0.), fPhiTwist(0.),
//    fAngleSide(0.), fDx4plus2(0.), fDx4minus2(0.), fDx3plus1(0.), fDx3minus1(0.),
//    fDy2plus1(0.), fDy2minus1(0.), fa1md1(0.), fa2md2(0.), fdeltaX(0.),
//    fdeltaY(0.)
//{
//}


//=====================================================================
//* destructor --------------------------------------------------------

SbTwistTrapAlphaSide::~SbTwistTrapAlphaSide()
{
}

//=====================================================================
//* GetFacets ---------------------------------------------------------

void
SbTwistTrapAlphaSide::GetFacets( int k, int n, double xyz[][3],
                                 int faces[][4], int iside )
{

  double phi ;
  double b ;

  double z, u ;     // the two parameters for the surface equation
  GeoTrf::Vector3D  p ;  // a point on the surface, given by (z,u)

  int nnode ;
  int nface ;

  // calculate the (n-1)*(k-1) vertices

  for ( int i = 0 ; i<n ; ++i )
  {
    z = -fDz+i*(2.*fDz)/(n-1) ;
    phi = z*fPhiTwist/(2*fDz) ;
    b = GetValueB(phi) ;

    for ( int j = 0 ; j<k ; ++j )
    {
      nnode = GetNode(i,j,k,n,iside) ;
      u = -b/2 +j*b/(k-1) ;
      p = SurfacePoint(phi,u,true) ;  // surface point in global coordinates

      xyz[nnode][0] = p.x() ;
      xyz[nnode][1] = p.y() ;
      xyz[nnode][2] = p.z() ;

      if ( i<n-1 && j<k-1 )  // conterclock wise filling
      {
        nface = GetFace(i,j,k,n,iside) ;
        faces[nface][0] = GetEdgeVisibility(i,j,k,n,0,-1)
                        * (GetNode(i  ,j  ,k,n,iside)+1) ;  // f77 numbering
        faces[nface][1] = GetEdgeVisibility(i,j,k,n,1,-1)
                        * (GetNode(i  ,j+1,k,n,iside)+1) ;
        faces[nface][2] = GetEdgeVisibility(i,j,k,n,2,-1)
                        * (GetNode(i+1,j+1,k,n,iside)+1) ;
        faces[nface][3] = GetEdgeVisibility(i,j,k,n,3,-1)
                        * (GetNode(i+1,j  ,k,n,iside)+1) ;
      }
    }
  }
}

