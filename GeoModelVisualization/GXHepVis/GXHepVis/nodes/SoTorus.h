/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/*-----------------------------HEPVis----------------------------------------*/
/*                                                                           */
/* Node:             SoTorus                                                 */
/* Description:      Represents the G4Torus Geant Geometry entity            */
/* Author:           Riccardo Maria BIANCHI, Jul 8 2019                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef HEPVis_SoTorus_h
#define HEPVis_SoTorus_h

#include <Inventor/C/errors/debugerror.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoShape.h>

#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoFaceSet.h>
// #include <Inventor/misc/SoRef.h> // TODO: this seems to be specific to  OpenInventor
// #include <Inventor/caches/SoNodeDependencies.h> // TODO: this seems to be specific to commercial OpenInventor

#include <fstream> // for debug


class SoSFNode;
//! SoTorus - Inventor version of the G4Torus Geant Geometry entity
/*!
 * Node:             SoTorus
 *
 * Description: The Inventor version of the G4Cons Geant Geometry entity
 *
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch> Jul 7, 2019
 *
 *
 * Shape class representing a torus.
 * The fRtor radius field controls the major radius of the torus (from the center).
 * The fRmax radius field controls the outer radius of the cross section of the torus.
 * The fRmin radius field controls the inner radius of the cross section of the torus.
 *
 * The documentation from Geant4 says:
 *
 * A torus or torus segment with curved sides parallel to the z-axis.
 * The torus has a specified swept radius about which it is centered,
 * and a given minimum and maximum radius. A minimum radius of 0
 * signifies a filled torus.
 * The torus segment is specified by starting and delta angles for phi,
 * with 0 being the +x axis, PI/2 the +y axis. A delta angle of 2PI
 * signifies a complete, unsegmented torus/cylindr.
 *
 * Member Data:
 *
 *      fRtor  radius of the torus itself, from the center
 *      fRmax  outside radius of the torus cross-section (the torus "tube")
 *      fRmin  inside radius  of the torus cross-section (Rmin=0 if not hollow)
 *
 *      fSPhi   starting angle of the segment in radians
 *      fDPhi   delta angle of the segment in radians
 *
 * Note: partially implemented from OpenInventor tutorial at:
 * - https://developer100.openinventor.com/content/26-creating-shape-node
 *
*/

class SoTorus:public SoShape {

  // The following is required:
  SO_NODE_HEADER(SoTorus);

public:

  //
  //! Inside radius
  //
  SoSFFloat fRInner;
  //
  //! Outside radius
  //
  SoSFFloat fRMinor;
  //
  //! Torus' radius
  //
  SoSFFloat fRMajor;
  //
  //! Starting angle, in radians
  //
  SoSFFloat fSPhi;
  //
  //! Delta-angle, in radians
  //
  SoSFFloat fDPhi;
  //
  //! An Inventor option - slightly better render, worse performance
  //
  SoSFBool  smoothDraw;
  //
  //! Override number of phi subdivision used for rendering shape (i.e. ignore e.g. complexity value).
  //! Put field to 0 (the default) to ignore it.
  //
  SoSFInt32 pOverrideNPhi;
  //
  //
  //! Constructor, required
  //
  SoTorus();

  //
  //! Class Initializer, required
  //
  static void initClass();

protected:

  //
  //! compute bounding Box, required
  //
  virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center );

  //
  //! Generate Primitives, required
  //
  virtual void generatePrimitives(SoAction *action);

  //
  //! GetChildList, required whenever the class has hidden children
  //
  virtual SoChildList *getChildren() const;

  virtual void doAction( SoAction* action );


protected:
  //
  //! Destructor, required
  //
  virtual ~SoTorus();

private:

  //
  //! Generate Children. Used to create the hidden children. Required whenever
  //! the node has hidden children.
  //
  void generateChildren();

  //
  //! Used to modify hidden children when a data field is changed. Required
  //! whenever the class has hidden children.
  //
  void updateChildren();

  //
  //! ChildList. Required whenever the class has hidden children.
  //
  SoChildList *m_children;

  //
  //! help with trigonometry.  increments sines an cosines by an angle.
  //
  void inc(double & sinPhi, double & cosPhi, double sinDeltaPhi, double cosDeltaPhi) const {
    double oldSin=sinPhi,oldCos=cosPhi;
    sinPhi = oldSin*cosDeltaPhi+oldCos*sinDeltaPhi;
    cosPhi = oldCos*cosDeltaPhi-oldSin*sinDeltaPhi;
  }
  static bool s_didInit;

  // Structure used to pass around information about how to draw the torus
  struct TorusInfo
  {
    int numc; // number of vertices around cross-section
    int numt; // number of vertices around torus
  };

  // These methods are used to compute the different vertex properties given
  // the current torus subdivision we are working on during shape construction.

  // SbVec3f getVertex( int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs );
  SbVec3f getVertex( double radius, int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs );

  SbVec2f getTexCoord( int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs );

  // SbVec3f getNormal( const SbVec3f& vert, int subdiv, int numSubdivs );
  SbVec3f getNormal( const SbVec3f& vert, int subdiv, int numSubdivs, bool doInvert=false );
  SbVec3f getNormalEndCap( const SbVec3f& vert, int subdiv, int numSubdivs, bool doInvert=false );



  // Uses the complexity element from the state to compute geometrical information
  // needed to build the internal shape.
  void computeComplexityInfo( SoState* state );

  // Retrieve internal shape representing the torus
  SoNode* getInternalShape( SoState* state );

  // Update internal shape geometry depending on the Torus field values.
  // void updateInternalShape( SoState* state );
  void updateInternalShape(SoState* state, SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxsection, bool isInnerVolume=false );

  // build an "endcap" shape, when building a toroidal segment
  // build a filled endcap
  void buildEndcaps(SoFaceSet* shape, SoVertexProperty* vertexProperty, double Rxs, int slice, bool invert=false);
  // build a pierced endcap
  void buildEndcaps(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs, double Rinner, int slice, bool invert=false);


  // vertex property node containing the geometry of the torus
  // SoRef<SoVertexProperty> m_vertexProperty; // TODO: this seems to be specific to  OpenInventor
  //SoVertexProperty* m_vertexProperty; // RMB: removed to stop the 'unused variable' warning

  // Internal shape representing the torus
  // SoRef<SoTriangleStripSet> m_internalShape; // TODO: this seems to be specific to  OpenInventor
  //SoTriangleStripSet* m_internalShape; // RMB: removed to stop the 'unused variable' warning

  // This cache handler is used as a flag to check whether or not the internal shape needs to be rebuilt.
  // It is also needed to add a dependency on one or more inventor elements, so that the cache
  // automatically invalidates itself when the element is changed.
  // In our case, we need a dependancy on the Complexity element and on both radii fields.
  // SoRef<SoNodeDependencies> m_internalShapeCache; // TODO: this seems to be specific to commercial OpenInventor

  // Use this structure to hold info about how to draw the torus
  TorusInfo m_info;

  // output data for debugging
  std::ofstream m_myfile;


};

#endif
