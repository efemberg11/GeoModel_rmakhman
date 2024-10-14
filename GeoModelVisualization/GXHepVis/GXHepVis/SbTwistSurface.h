//
// SbTwistSurface
//
// Class description:
//
// Abstract base class for twisted surfaces

// 01-Aug-2002 - Kotoyo Hoshina (hoshina@hepburn.s.chiba-u.ac.jp), created.
// 13-Nov-2003 - O.Link (Oliver.Link@cern.ch), Integration in Geant4
//               from original version in Jupiter-2.5.02 application.
// 08-Dec-2020 - M. Bandieramonte (marilena.bandieramonte@cern.ch),
//               adapted from Geant4 to GMEX
//
// --------------------------------------------------------------------
#ifndef SBTWISTSURFACE_HH
#define SBTWISTSURFACE_HH

#include <string>
#include "GeoModelKernel/GeoDefinitions.h"
#include <GXHepVis/SbRotation.h>
#include <Inventor/SbVec4d.h>

#define SBSURFACENXX 10

class SbTwistSurface
{
 public:  // without description

//   enum EValidate { kDontValidate = 0, kValidateWithTol = 1,
//                    kValidateWithoutTol = 2, kUninitialized = 3 };

 public:  // with description

   SbTwistSurface (const std::string& name);
   SbTwistSurface (const std::string& name,
                   const GeoTrf::Transform3D rotTrans,
                   int   handedness);
    
   virtual ~SbTwistSurface();
    
   virtual void GetFacets(int m, int n, double xyz[][3],
                          int faces[][4], int iside) = 0 ;
   int GetNode( int i, int j, int m, int n, int iside ) ;
   int GetFace( int i, int j, int m, int n, int iside ) ;
   int GetEdgeVisibility( int i, int j, int m, int n,
                            int number, int orientation) ;
   virtual std::string      GetName() const { return fName; }
   inline void SetNeighbours(SbTwistSurface* ax0min, SbTwistSurface* ax1min,
                                   SbTwistSurface* ax0max, SbTwistSurface* ax1max)
    {
       fNeighbours[0] = ax0min;
       fNeighbours[1] = ax1min;
       fNeighbours[2] = ax0max;
       fNeighbours[3] = ax1max;
    }
    
private:
    bool fIsValidNorm;
    std::string fName;
    SbTwistSurface* fNeighbours[4]; // {0,1,2,3} = sAxis0min, sAxis1min,
    
protected:
    int  fHandedness;
    GeoTrf::Transform3D fRotTrans;
    
};
#endif

