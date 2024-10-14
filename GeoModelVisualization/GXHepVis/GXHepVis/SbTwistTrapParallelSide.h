//
// SbTwistTrapParallelSide
//
// Class description:
//
// Class describing a twisted boundary surface for a trapezoid.

// Author:   27-Oct-2004 - O.Link (Oliver.Link@cern.ch)
// Revision: 08-Dec-2020 - Marilena Bandieramonte (marilena.bandieramonte@cern.ch):
// Adapted from Geant4 to GMEX
// --------------------------------------------------------------------
#ifndef SBTWISTTRAPPARALLELSIDE_HH
#define SBTWISTTRAPPARALLELSIDE_HH

#include "GXHepVis/SbTwistSurface.h"
#include "GXHepVis/SbPolyhedron.h"

#include <vector>
#include <cmath>
#include <iostream>

class SbTwistTrapParallelSide : public SbTwistSurface
{
  public:  // with description
   
    SbTwistTrapParallelSide(const std::string& name,
                              double  PhiTwist, // twist angle
                              double  pDz,      // half z lenght
                              double  pTheta, // direction between end planes
                              double  pPhi,   // by polar and azimutal angles
                              double  pDy1,     // half y length at -pDz
                              double  pDx1,     // half x length at -pDz,-pDy
                              double  pDx2,     // half x length at -pDz,+pDy
                              double  pDy2,     // half y length at +pDz
                              double  pDx3,     // half x length at +pDz,-pDy
                              double  pDx4,     // half x length at +pDz,+pDy
                              double  pAlph,    // tilt angle at +pDz
                              double  AngleSide // parity
                            );
  
    virtual ~SbTwistTrapParallelSide();
    
    inline
    double GetBoundaryMin(double phi)
    {
      return  -(fPhiTwist*(fDx2 + fDx4 - fDy2plus1*fTAlph)
              + 2*fDx4minus2*phi - 2*fDy2minus1*fTAlph*phi)/(2.*fPhiTwist) ;
    }

    inline
    double GetBoundaryMax(double phi)
    {
      return (fDx2 + fDx4 + fDy2plus1*fTAlph)/ 2.
           + ((fDx4minus2 + fDy2minus1*fTAlph)*phi)/fPhiTwist ;
    }
    inline
    double GetValueB(double phi)
    {
      return ( fDy2plus1 + fDy2minus1 * ( 2 * phi ) / fPhiTwist ) ;
    }
    
    inline
    double Xcoef(double phi)
    {
      return GetValueB(phi)/2. ;
    }
    
    inline GeoTrf::Vector3D
    SurfacePoint( double phi, double u, bool isGlobal )
    {
      // function to calculate a point on the surface, given by parameters phi,u

      GeoTrf::Vector3D SurfPoint ( u*std::cos(phi) - Xcoef(phi)*std::sin(phi)
                              + fdeltaX*phi/fPhiTwist,
                                u*std::sin(phi) + Xcoef(phi)*std::cos(phi)
                              + fdeltaY*phi/fPhiTwist,
                                2*fDz*phi/fPhiTwist  );
      if (isGlobal) { return (fRotTrans*SurfPoint); }
      return SurfPoint;
    }
    
private:
    
    virtual void GetFacets( int m, int n, double xyz[][3],
    int faces[][4], int iside );

    double fTheta;
    double fPhi ;

    double fDy1;
    double fDx1;
    double fDx2;

    double fDy2;
    double fDx3;
    double fDx4;

    double fDz;         // Half-length along the z axis

    double fAlph;
    double fTAlph;      // std::tan(fAlph)
    
    double fPhiTwist;   // twist angle (dphi in surface equation)

    double fAngleSide;

    double fDx4plus2;  // fDx4 + fDx2  == a2/2 + a1/2
    double fDx4minus2; // fDx4 - fDx2          -
    double fDx3plus1;  // fDx3 + fDx1  == d2/2 + d1/2
    double fDx3minus1; // fDx3 - fDx1          -
    double fDy2plus1;  // fDy2 + fDy1  == b2/2 + b1/2
    double fDy2minus1; // fDy2 - fDy1          -
    double fa1md1;     // 2 fDx2 - 2 fDx1  == a1 - d1
    double fa2md2;     // 2 fDx4 - 2 fDx3

    double fdeltaX;
    double fdeltaY;
};
#endif
