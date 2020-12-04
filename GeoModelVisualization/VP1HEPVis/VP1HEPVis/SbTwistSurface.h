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
// SbTwistSurface
//
// Class description:
//
// Abstract base class for boundary surface of G4VSolid.

// 01-Aug-2002 - Kotoyo Hoshina (hoshina@hepburn.s.chiba-u.ac.jp), created.
// 13-Nov-2003 - O.Link (Oliver.Link@cern.ch), Integration in Geant4
//               from original version in Jupiter-2.5.02 application.
//
// --------------------------------------------------------------------
#ifndef SBTWISTSURFACE_HH
#define SBTWISTSURFACE_HH

// Units
//#include "GeoModelKernel/Units.h"
//#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'
//#include <CLHEP/Units/SystemOfUnits.h>

//#include "G4VSolid.hh"
//#include "geomdefs.hh"

//#include "G4RotationMatrix.hh"
#include <string>
#include "GeoModelKernel/GeoDefinitions.h"
#include <VP1HEPVis/SbRotation.h>
#include <Inventor/SbVec4d.h>

class SbVec3d;

//class HVPoint3D : public SbVec3d {
//public:
//  HVPoint3D();
//  HVPoint3D(double x,double y,double z);
//  HVPoint3D(const HVPoint3D& v);
//  HVPoint3D(const SbVec3d& v);
//  HVPoint3D& operator=(const HVPoint3D& v);
//  HVPoint3D& operator=(const SbVec3d& v);
//  friend HVPoint3D operator +(const HVPoint3D& v1, const HVPoint3D& v2);
//};
//HVPoint3D operator +(const HVPoint3D& v1,const HVPoint3D& v2);
//typedef HVPoint3D HVNormal3D;
//typedef HVPoint3D HVVector3D;

#define SBSURFACENXX 10

class SbTwistSurface
{
 public:  // without description

//   enum EValidate { kDontValidate = 0, kValidateWithTol = 1,
//                    kValidateWithoutTol = 2, kUninitialized = 3 };

 public:  // with description

   SbTwistSurface (const std::string& name);
   SbTwistSurface (const std::string& name,
               const HEPVis::SbRotation& rot,
               const SbVec3d&    tlate,
                     int             handedness
//               const EAxis             axis1,
//               const EAxis             axis2,
//                     double          axis0min = -kInfinity,
//                     double          axis1min = -kInfinity,
//                     double          axis0max = kInfinity,
//                     double          axis1max = kInfinity
    );

   virtual ~SbTwistSurface();
    
   virtual void GetFacets(int m, int n, double xyz[][3],
                          int faces[][4], int iside) = 0 ;
   int GetNode( int i, int j, int m, int n, int iside ) ;
   int GetFace( int i, int j, int m, int n, int iside ) ;
   int GetEdgeVisibility( int i, int j, int m, int n,
                            int number, int orientation) ;
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
//    GeoTrf::RotationMatrix3D    fRot;
//    GeoTrf::Vector3D       fTrans; //TO DO CHECK correctness
};

//   virtual int     AmIOnLeftSide(const G4ThreeVector& me,
//                                   const G4ThreeVector& vec,
//                                         bool withTol = true);
//
//   virtual double  DistanceToBoundary(      int areacode,
//                                              G4ThreeVector& xx,
//                                        const G4ThreeVector& p) ;
//
//   virtual double  DistanceToIn(const G4ThreeVector& gp,
//                                  const G4ThreeVector& gv,
//                                        G4ThreeVector& gxxbest);
//   virtual double  DistanceToOut(const G4ThreeVector& gp,
//                                   const G4ThreeVector& gv,
//                                         G4ThreeVector& gxxbest);
//   virtual double  DistanceTo(const G4ThreeVector& gp,
//                                      G4ThreeVector& gxx);
//
//   virtual int     DistanceToSurface(const G4ThreeVector& gp,
//                                       const G4ThreeVector& gv,
//                                             G4ThreeVector gxx[],
//                                             double      distance[],
//                                             int         areacode[],
//                                             bool        isvalid[],
//                                       EValidate validate=kValidateWithTol) = 0;
//
//   virtual int     DistanceToSurface(const G4ThreeVector& gp,
//                                             G4ThreeVector gxx[],
//                                             double      distance[],
//                                             int         areacode[]) = 0;
//
//   void              DebugPrint() const;
//
//   // get methods
//
//   virtual G4ThreeVector GetNormal(const G4ThreeVector& xx,bool isGlobal) = 0;
//
//   virtual G4String      GetName() const { return fName; }
//   virtual void          GetBoundaryParameters(const int& areacode,
//                                               G4ThreeVector& d,
//                                               G4ThreeVector& x0,
//                                               int& boundarytype) const;
//   virtual G4ThreeVector GetBoundaryAtPZ(int areacode,
//                                         const G4ThreeVector& p) const;
//
//   inline  double DistanceToPlaneWithV(const G4ThreeVector& p,
//                                         const G4ThreeVector& v,
//                                         const G4ThreeVector& x0,
//                                         const G4ThreeVector& n0,
//                                               G4ThreeVector& xx);
//
//   inline  double DistanceToPlane(const G4ThreeVector& p,
//                                    const G4ThreeVector& x0,
//                                    const G4ThreeVector& n0,
//                                          G4ThreeVector& xx);
//
//   inline  double DistanceToPlane(const G4ThreeVector& p,
//                                    const G4ThreeVector& x0,
//                                    const G4ThreeVector& t1,
//                                    const G4ThreeVector& t2,
//                                          G4ThreeVector& xx,
//                                          G4ThreeVector& n);
//
//   inline  double DistanceToLine (const G4ThreeVector& p,
//                                    const G4ThreeVector& x0,
//                                    const G4ThreeVector& d,
//                                          G4ThreeVector& xx);
//
//   inline bool IsAxis0    (int areacode) const;
//   inline bool IsAxis1    (int areacode) const;
//   inline bool IsOutside  (int areacode) const;
//   inline bool IsInside   (int areacode, bool testbitmode = false) const;
//   inline bool IsBoundary (int areacode, bool testbitmode = false) const;
//   inline bool IsCorner   (int areacode, bool testbitmode = false) const;
//   inline bool IsValidNorm() const { return fIsValidNorm; }
//   bool IsSameBoundary (G4VTwistSurface* surface1, int areacode1,
//                          G4VTwistSurface* surface2, int areacode2 ) const;
//   inline int  GetAxisType(int areacode, int whichaxis) const;

//   inline G4ThreeVector ComputeGlobalPoint     (const G4ThreeVector& lp) const;
//   inline G4ThreeVector ComputeLocalPoint      (const G4ThreeVector& gp) const;
//   inline G4ThreeVector ComputeGlobalDirection (const G4ThreeVector& lp) const;
//   inline G4ThreeVector ComputeLocalDirection  (const G4ThreeVector& gp) const;
  
   // set methods

   //inline void SetAxis(int i, const EAxis axis)  { fAxis[i] = axis; }
//   inline void SetNeighbours(G4VTwistSurface* ax0min, G4VTwistSurface* ax1min,
//                             G4VTwistSurface* ax0max, G4VTwistSurface* ax1max);
//
//   virtual G4ThreeVector SurfacePoint(double, double,
//                                      bool isGlobal = false ) = 0 ;
//   virtual double GetBoundaryMin(double) = 0 ;
//   virtual double GetBoundaryMax(double) = 0 ;
//   virtual double GetSurfaceArea() = 0 ;
//   virtual void GetFacets(int m, int n, double xyz[][3],
//                          int faces[][4], int iside) = 0 ;
//   int GetNode( int i, int j, int m, int n, int iside ) ;
//   int GetFace( int i, int j, int m, int n, int iside ) ;
//   int GetEdgeVisibility( int i, int j, int m, int n,
//                            int number, int orientation) ;
//
// public:  // without description
//
//   G4VTwistSurface(__void__&);
//     // Fake default constructor for usage restricted to direct object
//     // persistency for clients requiring preallocation of memory for
//     // persistifiable objects.
//
// protected:  // with description
//
//   // get methods
//
//   inline G4VTwistSurface** GetNeighbours() { return fNeighbours; }
//   inline int GetNeighbours(int areacode, G4VTwistSurface* surfaces[]);
//   inline G4ThreeVector GetCorner(int areacode) const;
//          void GetBoundaryAxis(int areacode, EAxis axis[]) const;
//          void GetBoundaryLimit(int areacode, double limit[]) const;
//   virtual int GetAreaCode(const G4ThreeVector& xx, bool withtol=true) = 0;
//
//   // set methods
//
//   virtual void SetBoundary(const int&         axiscode,
//                            const G4ThreeVector& direction,
//                            const G4ThreeVector& x0,
//                            const int&         boundarytype);
//     // areacode must be one of them:
//     // sAxis0 & sAxisMin, sAxis0 & sAxisMax,
//     // sAxis1 & sAxisMin, sAxis1 & sAxisMax.
//     // boundarytype represents the shape of locus
//     // from the start point to end point of boundary.
//     // ex.
//     // sAxisRho = linear line which start point is fixed at origin.
//     // sAxisPhi = part of circle which center placed at the origin.
//
//   void SetCorner(int areacode, double x, double y, double z);
//
// private:
//
//   virtual void SetBoundaries() = 0;
//   virtual void SetCorners()    = 0;
//
// // data members ---------------------------------------------------------
//
// public:
//
//   static const int sOutside ;
//   static const int sInside  ;
//   static const int sBoundary;
//   static const int sCorner;
//   static const int sC0Min1Min;
//   static const int sC0Max1Min;
//   static const int sC0Max1Max;
//   static const int sC0Min1Max;
//   static const int sAxisMin;
//   static const int sAxisMax;
//   static const int sAxisX;
//   static const int sAxisY;
//   static const int sAxisZ;
//   static const int sAxisRho;
//   static const int sAxisPhi;
//   static const int sAxis0;
//   static const int sAxis1;
//   static const int sSizeMask;
//   static const int sAxisMask;
//   static const int sAreaMask;
//
// protected:
//
//   class CurrentStatus
//   {
//    public:
//
//      CurrentStatus();
//      virtual ~CurrentStatus();
//
//      inline G4ThreeVector GetXX(int i)       const { return fXX[i];       }
//      inline double      GetDistance(int i) const { return fDistance[i]; }
//      inline int         GetAreacode(int i) const { return fAreacode[i]; }
//      inline int         GetNXX()             const { return fNXX;         }
//      inline bool        IsDone()             const { return fDone;        }
//      inline bool        IsValid(int i)     const { return fIsValid[i];  }
//
//      void SetCurrentStatus(int                i,
//                            G4ThreeVector&       xx,
//                            double&            dist,
//                            int&               areacode,
//                            bool&              isvalid,
//                            int                nxx,
//                            EValidate            validate,
//                      const G4ThreeVector* p,
//                      const G4ThreeVector* v = nullptr);
//
//      void ResetfDone(EValidate validate,
//                const G4ThreeVector* p,
//                const G4ThreeVector* v = nullptr);
//
//
//      void DebugPrint() const;
//
//    private:
//
//      double             fDistance[SBSURFACENXX];
//      G4ThreeVector        fXX[SBSURFACENXX];
//      int                fAreacode[SBSURFACENXX];
//      bool               fIsValid[SBSURFACENXX];
//      int                fNXX;
//      G4ThreeVector        fLastp;
//      G4ThreeVector        fLastv;
//      EValidate            fLastValidate;
//      bool               fDone;
//   };
//
//   class Boundary
//   {
//    public:
//      Boundary();
//      virtual ~Boundary();
//
//      void SetFields(const int&        areacode,
//                     const G4ThreeVector& d,
//                     const G4ThreeVector& x0,
//                     const int&         boundarytype);
//
//      bool IsEmpty() const;
//
//      bool GetBoundaryParameters(const int&         areacode,
//                                         G4ThreeVector& d,
//                                         G4ThreeVector& x0,
//                                         int&         boundarytype) const;
//
//    private:
//      int          fBoundaryAcode;
//      G4ThreeVector  fBoundaryDirection;
//      G4ThreeVector  fBoundaryX0;
//      int          fBoundaryType;
//   };
//
//   //EAxis               fAxis[2];
//   double            fAxisMin[2];
//   double            fAxisMax[2];
//   //CurrentStatus       fCurStatWithV;
//   //CurrentStatus       fCurStat;
//   G4RotationMatrix    fRot;
//   G4ThreeVector       fTrans;

//   class G4SurfCurNormal
//   {
//     public:
//       G4ThreeVector p;
//       G4ThreeVector normal;
//   };
//   //G4SurfCurNormal     fCurrentNormal;
//   bool              fIsValidNorm;
//   double            kCarTolerance;
//
// private:
//
//   SbTwistSurface* fNeighbours[4]; // {0,1,2,3} = sAxis0min, sAxis1min,
//                                    //             sAxis0max, sAxis1max
//   G4ThreeVector fCorners[4];     // corners of the surface in local coordinate
//  //Boundary      fBoundaries[4];  // boundaries of the surface.
//   G4String      fName;
   
//   class G4SurfSideQuery
//   {
//     public:
//       G4ThreeVector me;
//       G4ThreeVector vec;
//       bool        withTol;
//       int         amIOnLeftSide;
//   };
//   G4SurfSideQuery fAmIOnLeftSide;
//};

//========================================================
// inline functions
//========================================================

//struct Intersection
//{
//  double phi ;  // parameter phi
//  double u ;    // parameter u
//  G4ThreeVector xx ;   // intersection point in cartesian
//  double distance ;  // distance to intersection
//  int areacode ;     // the areacode of the intersection
//  bool isvalid ;     // valid intersection ??
//
//};
//
//inline
//bool DistanceSort( const Intersection& a, const Intersection& b)
//{
//  return a.distance < b.distance ;
//}
//
//inline
//bool EqualIntersection( const Intersection& a, const Intersection& b)
//{
//  return ( ( a.xx - b.xx ).mag() < 1E-9*CLHEP::mm ) ;
//}

//#include "G4VTwistSurface.icc"

#endif

