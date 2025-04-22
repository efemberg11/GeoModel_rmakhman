//
// G4TwistTrapAlphaSide implementation
//
// Author:   18/03/2005 - O.Link (Oliver.Link@cern.ch)
// Revision: 08/Dec/2020 - Marilena Bandieramonte (marilena.bandieramonte@cern.ch):
// Adapted from Geant4 to GMEX
// --------------------------------------------------------------------

#include <cmath>
#include <iostream>

#include "GXHepVis/SbTwistTrapAlphaSide.h"

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

  fdeltaX = 2 * fDz * std::tan(fTheta) * std::cos(fPhi);
    // dx in surface equation
  fdeltaY = 2 * fDz * std::tan(fTheta) * std::sin(fPhi);
    // dy in surface equation
    
  GeoTrf::Transform3D rotation = GeoTrf::RotateZ3D(AngleSide);
  GeoTrf::Vector3D translation=GeoTrf::Vector3D(0,0,0);
  fRotTrans = GeoTrf::Translate3D(translation.x(),translation.y(),translation.z())*rotation;
    
}


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

