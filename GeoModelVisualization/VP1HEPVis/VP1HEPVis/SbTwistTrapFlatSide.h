//
// SbFlatTrapSurface
//
// Class description:
//
// Class describing a flat boundary surface for a trapezoid.
//
// Author:   27-Oct-2004 - O.Link (Oliver.Link@cern.ch)
// Revision: 08-Dec-2020 -  Marilena Bandieramonte (marilena.bandieramonte@cern.ch):
// Adapted from Geant4 to GMEX
// --------------------------------------------------------------------
#ifndef SBTWISTTRAPFLATSIDE_HH
#define SBTWISTTRAPFLATSIDE_HH

#include "VP1HEPVis/SbTwistSurface.h"
#include <string>

class SbTwistTrapFlatSide : public SbTwistSurface
{
  public:  // with description

   SbTwistTrapFlatSide( const std::string& name,
                              double  PhiTwist,
                              double  pDx1,
                              double  pDx2,
                              double  pDy,
                              double  pDz,
                              double  pAlpha,
                              double  pPhi,
                              double  pTheta,
                              int     handedness  );
   virtual ~SbTwistTrapFlatSide();
    
   virtual void GetFacets( int m, int n, double xyz[][3],
                             int faces[][4], int iside );
    inline
    double xAxisMax(double u, double fTanAlpha) const
    {
      return (  ( fDx2 + fDx1 )/2. + u*(fDx2 - fDx1)/(2.*fDy) + u *fTanAlpha  ) ;
    }
    
    inline GeoTrf::Vector3D
    SurfacePoint(double x, double y, bool isGlobal)
    {
      GeoTrf::Vector3D SurfPoint ( x,y,0);
      if (isGlobal) { return (fRotTrans*SurfPoint); }
      return SurfPoint;
    }

    inline
    double GetBoundaryMin(double y)
    {
      return -xAxisMax(y, -fTAlph) ;
    }

    inline
    double GetBoundaryMax(double y)
    {
      return xAxisMax(y, fTAlph) ;
    }
 private:
    
     double fDx1;
     double fDx2;
     double fDy;
     double fDz;
     double fPhiTwist;
     double fAlpha;
     double fTAlph;
     double fPhi;
     double fTheta;
     double fdeltaX;
     double fdeltaY;
};
#endif
