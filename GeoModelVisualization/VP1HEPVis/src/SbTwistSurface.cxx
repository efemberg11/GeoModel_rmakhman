//
// G4VTwistSurface implementation
//
// 01-Aug-2002 - Kotoyo Hoshina (hoshina@hepburn.s.chiba-u.ac.jp), created.
// 13-Nov-2003 - O.Link (Oliver.Link@cern.ch), Integration in Geant4
//               from original version in Jupiter-2.5.02 application.
// 08-Dec-2020 - M. Bandieramonte (Marilena.bandieramonte@cern.ch)
//               Adapted from Geant4 to GMEX
// --------------------------------------------------------------------

#include <iomanip>
#include <iostream>

#include "VP1HEPVis/SbTwistSurface.h"

//=====================================================================
//* constructors ------------------------------------------------------

SbTwistSurface::SbTwistSurface(const std::string &name)
  : fIsValidNorm(false), fName(name)
{
    fHandedness = 1;
    fRotTrans = GeoTrf::Transform3D::Identity();
    for (auto i=0; i<4; ++i)
    {
      fNeighbours[i] = nullptr;
        
    }

}

SbTwistSurface::SbTwistSurface(const std::string& name,
                               const GeoTrf::Transform3D rotTrans,
                               int   handedness): fIsValidNorm(false), fName(name)
{
    fHandedness = handedness;
    fRotTrans =rotTrans;
    for (auto i=0; i<4; ++i)
    {
        fNeighbours[i] = nullptr;
        
    }
}


//=====================================================================
//* destructor --------------------------------------------------------

SbTwistSurface::~SbTwistSurface()
{
}

//=====================================================================
//* GetNode -----------------------------------------------------------

int SbTwistSurface::GetNode( int i, int j, int k,
                                int n, int iside )
{
  // this is the node mapping function
  // (i,j) -> node number
  // Depends on the side iside and the used meshing of the surface

  if ( iside == 0 )
  {
    // lower endcap is kxk squared.
    // n = k
    return i * k + j ;
  }

  if ( iside == 1 )
  {
    // upper endcap is kxk squared. Shift by k*k
    // n = k
    return  k*k + i*k + j ;
  }

  else if ( iside == 2 )
  {
    // front side.
    if      ( i == 0 )   { return       j ;  }
    else if ( i == n-1 ) { return k*k + j ;  }
    else                 { return 2*k*k + 4*(i-1)*(k-1) + j ; }
  }

  else if ( iside == 3 )
  {
    // right side
    if      ( i == 0 )   { return       (j+1)*k - 1 ; }
    else if ( i == n-1 ) { return k*k + (j+1)*k - 1 ; }
    else
    {
      return 2*k*k + 4*(i-1)*(k-1) + (k-1) + j ;
    }
  }
  else if ( iside == 4 )
  {
    // back side
    if      ( i == 0 )   { return   k*k - 1 - j ; }    // reversed order
    else if ( i == n-1 ) { return 2*k*k - 1 - j ; }    // reversed order
    else
    {
      return 2*k*k + 4*(i-1)*(k-1) + 2*(k-1) + j ; // normal order
    }
  }
  else if ( iside == 5 )
  {
    // left side
    if      ( i == 0 )   { return k*k   - (j+1)*k ; }  // reversed order
    else if ( i == n-1)  { return 2*k*k - (j+1)*k ; }  // reverded order
    else
    {
      if ( j == k-1 ) { return 2*k*k + 4*(i-1)*(k-1) ; } // special case
      else
      {
        return 2*k*k + 4*(i-1)*(k-1) + 3*(k-1) + j ; // normal order
      }
    }
  }
  else
  {
    std::cout << "Not correct side number: "
            << GetName() << std::endl
            << "iside is " << iside << " but should be "
            << "0,1,2,3,4 or 5" << ".";
  }
  return -1 ;  // wrong node
}

//=====================================================================
//* GetFace -----------------------------------------------------------

int SbTwistSurface::GetFace( int i, int j, int k,
                                int n, int iside )
{
  // this is the face mapping function
  // (i,j) -> face number

  if ( iside == 0 ) {
    return i * ( k - 1 ) + j ;
  }

  else if ( iside == 1 ) {
    return (k-1)*(k-1) + i*(k-1) + j ;
  }

  else if ( iside == 2 ) {
    return 2*(k-1)*(k-1) + i*(k-1) + j ;
  }

  else if ( iside == 3 ) {
    return 2*(k-1)*(k-1) + (n-1)*(k-1) + i*(k-1) + j ;
  }
  
  else if ( iside == 4 ) {
    return 2*(k-1)*(k-1) + 2*(n-1)*(k-1) + i*(k-1) + j ;
  }
  
  else if ( iside == 5 ) {
    return 2*(k-1)*(k-1) + 3*(n-1)*(k-1) + i*(k-1) + j ;
  }

  else {
      std::cout << "ERROR! Not correct side number: "
      << GetName() << std::endl;
      std::cout << "iside is " << iside << " but should be "
              << "0,1,2,3,4 or 5" << "."<< std::endl;
  }

  return -1 ;  // wrong face
}

//=====================================================================
//* GetEdgeVisiblility ------------------------------------------------

int SbTwistSurface::GetEdgeVisibility( int i, int j, int k, int n,
                                          int number, int orientation )
{
  // clockwise filling         -> positive orientation
  // counter clockwise filling -> negative orientation

  //
  //   d    C    c
  //     +------+
  //     |      |
  //     |      |
  //     |      |
  //   D |      |B
  //     |      |
  //     |      |
  //     |      |
  //     +------+
  //    a   A    b
  //
  //  a = +--+    A = ---+
  //  b = --++    B = --+-
  //  c = -++-    C = -+--
  //  d = ++--    D = +---


  // check first invisible faces

  if ( ( i>0 && i<n-2 ) && ( j>0 && j<k-2 ) )
  {
    return -1 ;   // always invisible, signs:   ----
  }
  
  // change first the vertex number (depends on the orientation)
  // 0,1,2,3  -> 3,2,1,0
  if ( orientation < 0 ) { number = ( 3 - number ) ;  }
  
  // check true edges
  if ( ( j>=1 && j<=k-3 ) )
  {
    if ( i == 0 )  {        // signs (A):  ---+
      return ( number == 3 ) ? 1 : -1 ;
    }
    
    else if ( i == n-2 ) {  // signs (C):  -+--
      return  ( number == 1 ) ? 1 : -1 ;
    }
    
    else
    {
      std::cout << "Not correct face number: " << GetName() << " !"<<std::endl;
    
    }
  }
  
  if ( ( i>=1 && i<=n-3 ) )
  {
    if ( j == 0 )  {        // signs (D):  +---
      return ( number == 0 ) ? 1 : -1 ;
    }

    else if ( j == k-2 ) {  // signs (B):  --+-
      return  ( number == 2 ) ? 1 : -1 ;
    }

    else
    {
      std::cout << "Not correct face number: " << GetName() << " !"<<std::endl;
    }
  }
  
  // now the corners
  if ( i == 0 && j == 0 ) {          // signs (a) : +--+
    return ( number == 0 || number == 3 ) ? 1 : -1 ;
  }
  else if ( i == 0 && j == k-2 ) {   // signs (b) : --++
    return ( number == 2 || number == 3 ) ? 1 : -1 ;
  }
  else if ( i == n-2 && j == k-2 ) { // signs (c) : -++-
    return ( number == 1 || number == 2 ) ? 1 : -1 ;
  }
  else if ( i == n-2 && j == 0 ) {   // signs (d) : ++--
    return ( number == 0 || number == 1 ) ? 1 : -1 ;
  }
  else
  {
      std::cout << "Not correct face number: " << GetName() << " !"<<std::endl;
  }
  std::cout << "Not correct face number: " << GetName() << " !"<<std::endl;

  return 0 ;
}
