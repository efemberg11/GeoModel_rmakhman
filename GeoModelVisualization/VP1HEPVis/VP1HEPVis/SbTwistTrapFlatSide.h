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
// SbFlatTrapSurface
//
// Class description:
//
// Class describing a flat boundary surface for a trapezoid.

// Author: 27-Oct-2004 - O.Link (Oliver.Link@cern.ch)
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

      //TO DO  SOLVE //// if (isGlobal) { return (fRot*SurfPoint + fTrans); }
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
