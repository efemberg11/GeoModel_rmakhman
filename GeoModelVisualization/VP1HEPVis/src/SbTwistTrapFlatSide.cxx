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
// SbTwistTrapFlatSide implementation
//
// Author: 30-Aug-2002 - O.Link (Oliver.Link@cern.ch)
// --------------------------------------------------------------------

#include "VP1HEPVis/SbTwistTrapFlatSide.h"
#include <cmath>

//=====================================================================
//* constructors ------------------------------------------------------

SbTwistTrapFlatSide::SbTwistTrapFlatSide( const std::string& name,
                                                double  PhiTwist,
                                                double  pDx1,
                                                double  pDx2,
                                                double  pDy,
                                                double  pDz,
                                                double  pAlpha,
                                                double  pPhi,
                                                double  pTheta,
                                                int     handedness)
  : SbTwistSurface(name)
{
   fHandedness = handedness;   // +z = +ve, -z = -ve

   fDx1 = pDx1 ;
   fDx2 = pDx2 ;
   fDy = pDy ;
   fDz = pDz ;
   fAlpha = pAlpha ;
   fTAlph = std::tan(fAlpha) ;
   fPhi  = pPhi ;
   fTheta = pTheta ;

   fdeltaX = 2 * fDz * std::tan(fTheta) * std::cos(fPhi)  ;
     // dx in surface equation
   fdeltaY = 2 * fDz * std::tan(fTheta) * std::sin(fPhi)  ;
     // dy in surface equation

   fPhiTwist = PhiTwist ;

//   fCurrentNormal.normal.set( 0, 0, (fHandedness < 0 ? -1 : 1));
//         // Unit vector, in local coordinate system
//   fRot.rotateZ( fHandedness > 0
//                 ? 0.5 * fPhiTwist
//                 : -0.5 * fPhiTwist );

//   fTrans.set(
//              fHandedness > 0 ? 0.5*fdeltaX : -0.5*fdeltaX ,
//              fHandedness > 0 ? 0.5*fdeltaY : -0.5*fdeltaY ,
//              fHandedness > 0 ? fDz : -fDz ) ;
//
//   fIsValidNorm = true;
//
//
//   fAxis[0] = kXAxis ;
//   fAxis[1] = kYAxis ;
//   fAxisMin[0] = kInfinity ;  // x-Axis cannot be fixed, because it
//   fAxisMax[0] =  kInfinity ; // depends on y
//   fAxisMin[1] = -fDy ;  // y - axis
//   fAxisMax[1] =  fDy ;
//
//   SetCorners();
//   SetBoundaries();
}


////=====================================================================
////* Fake default constructor ------------------------------------------
//
//G4TwistTrapFlatSide::G4TwistTrapFlatSide( __void__& a )
//  : G4VTwistSurface(a), fDx1(0.), fDx2(0.), fDy(0.), fDz(0.), fPhiTwist(0.),
//    fAlpha(0.), fTAlph(0.), fPhi(0.), fTheta(0.), fdeltaX(0.), fdeltaY(0.)
//{
//}


//=====================================================================
//* destructor --------------------------------------------------------

SbTwistTrapFlatSide::~SbTwistTrapFlatSide()
{
}


//=====================================================================
//* GetFacets() -------------------------------------------------------

void SbTwistTrapFlatSide::GetFacets( int k, int n, double xyz[][3],
                                     int faces[][4], int iside )
{
  double x,y    ;     // the two parameters for the surface equation
  GeoTrf::Vector3D  p ;  // a point on the surface, given by (z,u)

  int nnode ;
  int nface ;

  double xmin,xmax ;

  // calculate the (n-1)*(k-1) vertices

  for ( int i = 0 ; i<n ; ++i )
  {
    y = -fDy + i*(2*fDy)/(n-1) ;

    for ( int j = 0 ; j<k ; ++j )
    {
      xmin = GetBoundaryMin(y) ;
      xmax = GetBoundaryMax(y) ;
      x = xmin + j*(xmax-xmin)/(k-1) ;

      nnode = GetNode(i,j,k,n,iside) ;
      p = SurfacePoint(x,y,true) ;  // surface point in global coordinate system

      xyz[nnode][0] = p.x() ;
      xyz[nnode][1] = p.y() ;
      xyz[nnode][2] = p.z() ;

      if ( i<n-1 && j<k-1 )
      {
        nface = GetFace(i,j,k,n,iside) ;

        if (fHandedness < 0)  // lower side
        {
          faces[nface][0] = GetEdgeVisibility(i,j,k,n,0,1)
                          * ( GetNode(i  ,j  ,k,n,iside)+1) ;
          faces[nface][1] = GetEdgeVisibility(i,j,k,n,1,1)
                          * ( GetNode(i+1,j  ,k,n,iside)+1) ;
          faces[nface][2] = GetEdgeVisibility(i,j,k,n,2,1)
                          * ( GetNode(i+1,j+1,k,n,iside)+1) ;
          faces[nface][3] = GetEdgeVisibility(i,j,k,n,3,1)
                          * ( GetNode(i  ,j+1,k,n,iside)+1) ;
        }
        else                  // upper side
        {
          faces[nface][0] = GetEdgeVisibility(i,j,k,n,0,-1)
                          * ( GetNode(i  ,j  ,k,n,iside)+1) ;
          faces[nface][1] = GetEdgeVisibility(i,j,k,n,1,-1)
                          * ( GetNode(i  ,j+1,k,n,iside)+1) ;
          faces[nface][2] = GetEdgeVisibility(i,j,k,n,2,-1)
                          * ( GetNode(i+1,j+1,k,n,iside)+1) ;
          faces[nface][3] = GetEdgeVisibility(i,j,k,n,3,-1)
                          * ( GetNode(i+1,j  ,k,n,iside)+1) ;
        }
      }
    }
  }
}

