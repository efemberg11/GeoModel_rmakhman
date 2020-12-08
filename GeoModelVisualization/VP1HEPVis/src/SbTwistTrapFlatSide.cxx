//
// SbTwistTrapFlatSide implementation
//
// Author:   30-Aug-2002 - O.Link (Oliver.Link@cern.ch)
// Revision: 08-Dec-2020 - Marilena Bandieramonte (marilena.bandieramonte@cern.ch):
// Adapted from Geant4 to GMEX
// --------------------------------------------------------------------

#include "VP1HEPVis/SbTwistTrapFlatSide.h"
#include <cmath>
#include <iostream> 

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
    
   GeoTrf::Transform3D rotation = GeoTrf::RotateZ3D(fHandedness > 0 ? 0.5 * fPhiTwist : -0.5 * fPhiTwist );
   GeoTrf::Vector3D translation=GeoTrf::Vector3D(fHandedness > 0 ? 0.5*fdeltaX : -0.5*fdeltaX,fHandedness > 0 ? 0.5*fdeltaY : -0.5*fdeltaY,fHandedness > 0 ? fDz : -fDz);
   fRotTrans = GeoTrf::Translate3D(translation.x(),translation.y(),translation.z())*rotation;
    
}

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

