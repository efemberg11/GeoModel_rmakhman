/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

/*-----------------------------HEPVis---------------------------------------*/
/*                                                                          */
/* Node:             SoTorus                                                */
/* Description:      Represents the G4Torus Geant Geometry entity           */
/* Author:           Riccardo Maria BIANCHI, Jul 8 2019                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/

// this :
#include <VP1HEPVis/nodes/SoTorus.h>
#include "RevolutionSurfaceUtil.h"

#include <cassert>
#include <cmath>
// #include <Inventor/SbBox.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

// #include <Inventor/SoDB.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/misc/SoState.h>

#include "SbMath.h"

#include <iostream>
#include <iomanip>


// This statement is required
SO_NODE_SOURCE(SoTorus)


// TODO: add this to SbMath.h ??
#ifndef M_PI
  #define M_PI 3.14159265358979323846
  #define M_PI_2 	1.57079632679489661923
#endif
#define TWOPI (2.0 * M_PI)

// #define SPHERE_NUM_SLICES 30.0f
// #define SPHERE_NUM_STACKS 30.0f



// TODO: add this to SbMath.h
template <typename T> inline T Max(T a, T b) { return (a>b)?a:b; }
template <typename T> inline T Min(T a, T b) { return (a<b)?a:b; }
template <typename T> inline T Clamp(T a, T minV, T maxV) { return Min(Max(minV, a), maxV); }






//____________________________________________________________________
// Constructor
SoTorus::SoTorus()
  // : m_internalShapeCache( new SoNodeDependencies )
  : m_internalShape(nullptr)
{
  // This statement is required
  SO_NODE_CONSTRUCTOR(SoTorus);

  // Data fields are initialized like this:
  SO_NODE_ADD_FIELD(fRInner,               (0.0));
  SO_NODE_ADD_FIELD(fRMinor,               (1.0));
  SO_NODE_ADD_FIELD(fRMajor,               (1.0));
  SO_NODE_ADD_FIELD(fSPhi,                (0.0));
  SO_NODE_ADD_FIELD(fDPhi,             ((float)(2*M_PI)));
  SO_NODE_ADD_FIELD(smoothDraw,          (TRUE));
  SO_NODE_ADD_FIELD(pOverrideNPhi,       (0));
  SO_NODE_ADD_FIELD(alternateRep,        (NULL));
  m_children = new SoChildList(this);

  setNodeType(EXTENSION);

  // Add dependency to the torus fields. This allows the cache handler
  // to automatically invalidate itself when the fields are changed.
  // m_internalShapeCache->addDependency( fRMajor );
  // m_internalShapeCache->addDependency( fRMinor );
  // m_internalShapeCache->addDependency( fRInner );
  // m_internalShapeCache->addDependency( fSPhi );
  // m_internalShapeCache->addDependency( fDPhi );

  // This is where we add the dependancy to the complexity element.
   // function, we'll need an SoTextureCoordinateElement.
  // This allows the cache handler to automatically invalidate
  // itself when the complexity element is changed.
  // m_internalShapeCache->addDependency<SoComplexityElement>();

  // output data file for debug
  m_myfile.open ("vertices.csv");

}

//____________________________________________________________________
// Destructor
SoTorus::~SoTorus() {
  delete m_children;

  m_myfile.close();
}


//____________________________________________________________________
// Build the internal shape depending on the elements from the current state
void
// SoTorus::updateInternalShape( SoState* state )
SoTorus::updateInternalShape(SoState* state, SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double rXs, bool isInnerVolume)
{
  // Figure out how many polygons to use
  computeComplexityInfo( state );

  // // debug test, it builds a pentagon
  // m_info.numt = 5;
  // m_info.numc = 4;

  // Each triangle strip goes around the top view
  // Number of vertices per strip = twice the number of (major subdivisions + 1)
  // because we want to loop back to the beginning for the full toroid
  const int verticesPerStrip = 2 * (m_info.numt + 1);

  // Number of triangle strips = number of minor subdivisions
  const int numStrips = m_info.numc;

  // Set the numVertices field of the single TriangleStripSet accordingly
  shape->numVertices.setNum( numStrips );
  int32_t* numVertices = shape->numVertices.startEditing();
  for ( int strip = 0; strip < numStrips; strip++ )
    numVertices[strip] = verticesPerStrip; // set the number of vertices of each strip
  shape->numVertices.finishEditing();

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerStrip * numStrips;
  vertexProperty->vertex.setNum( numVerticesTotal );
  vertexProperty->normal.setNum( numVerticesTotal );
  vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = vertexProperty->vertex.startEditing();
  SbVec3f* normals   = vertexProperty->normal.startEditing();
  SbVec2f* texCoords = vertexProperty->texCoord.startEditing();

  // Now fill the buffers
  int vertexIndex = 0;

  // go around cross section
  for ( int strip = 0; strip < numStrips; strip++ )
  {
    // go around top view
    for ( int stripVertex = 0; stripVertex <= m_info.numt; stripVertex++ )
    {
      // each strip have two vertices per stripVertex: one on the top line and one on the bottom line
      // i.e. strip 0 has the stripVertex 0 vertices on the line 0 and 1,
      //      strip 1 has the stripVertex 0 vertices on the line 1 and 2,
      //      and so forth...
      for ( int offset = 1; offset >= 0; offset-- )
      {
        const int crossSection = strip + offset;

        vertices[vertexIndex]  = getVertex( rXs, crossSection, m_info.numc, stripVertex, m_info.numt );
        normals[vertexIndex]   = getNormal( vertices[vertexIndex], stripVertex, m_info.numt, isInnerVolume );
        texCoords[vertexIndex] = getTexCoord( crossSection, m_info.numc, stripVertex, m_info.numt );

        vertexIndex++;
      }
    }
  }
  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();
}

/*
//____________________________________________________________________
// Retrieve internal shape representing the torus
SoNode*
SoTorus::getInternalShape( SoState* state )
{
  // Initialize internal shape
  if ( m_internalShape == nullptr )
  {
    m_vertexProperty = new SoVertexProperty;
    m_vertexProperty->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    m_vertexProperty->materialBinding.setValue( SoVertexProperty::OVERALL );

    m_internalShape = new SoTriangleStripSet;
    m_internalShape->vertexProperty.setValue( m_vertexProperty );
  }

  // TODO:
  // If the cache handler is invalid, we need an update
  // if ( !m_internalShapeCache->isValid(state) )
  // {
    updateInternalShape( state );
    // m_internalShapeCache->updateCache( state ); // make the cache valid again after the update
  // }

  return m_internalShape;
} */


//____________________________________________________________________
// Retrieve internal shape representing the torus
SoNode*
SoTorus::getInternalShape( SoState* state )
{
  // // Initialize internal shape
  // if ( m_internalShape == nullptr )
  // {
  //   m_vertexProperty = new SoVertexProperty;
  //   m_vertexProperty->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
  //   m_vertexProperty->materialBinding.setValue( SoVertexProperty::OVERALL );
  //
  //   m_internalShape = new SoTriangleStripSet;
  //   m_internalShape->vertexProperty.setValue( m_vertexProperty );
  // }

  SoSeparator *sep = new SoSeparator;
  sep->ref();

  SoVertexProperty* vertexProperty = new SoVertexProperty;
  vertexProperty->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
  vertexProperty->materialBinding.setValue( SoVertexProperty::OVERALL ); // so the face is enlightened on both face

  SoTriangleStripSet* shape = new SoTriangleStripSet;
  shape->vertexProperty.setValue( vertexProperty );

  updateInternalShape( state, shape, vertexProperty, fRMinor.getValue() );
  sep->addChild(shape);


  // if rInner is set to 0, then we add filled endcaps
  if (fRInner.getValue() == 0) {

    std::cout << "\nBuild starting endcap..." <<std::endl;
    // first endcap, at the beginning of the toroidal segment
    SoVertexProperty* vertexProperty_endcaps_a = new SoVertexProperty;
    vertexProperty_endcaps_a->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_a->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoFaceSet* shape_endcaps_a = new SoFaceSet;
    shape_endcaps_a->vertexProperty.setValue( vertexProperty_endcaps_a );
    buildEndcaps( shape_endcaps_a, vertexProperty_endcaps_a, fRMinor.getValue(), 0, true );
    sep->addChild(shape_endcaps_a);

    std::cout << "\nBuild ending endcap..." <<std::endl;
    // second endcap, at the end of the toroidal segment
    SoVertexProperty* vertexProperty_endcaps_b = new SoVertexProperty;
    vertexProperty_endcaps_b->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_b->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoFaceSet* shape_endcaps_b = new SoFaceSet;
    shape_endcaps_b->vertexProperty.setValue( vertexProperty_endcaps_b );
    buildEndcaps( shape_endcaps_b, vertexProperty_endcaps_b, fRMinor.getValue(), m_info.numt);
    // sep->addChild(shape_endcaps_b);
  }
  // if rInner is set, we build a second, inner torus and pierced endcaps
  else if (fRInner.getValue() != -1) {

    // add a second, inner torus
    SoVertexProperty* vertexProperty_inner = new SoVertexProperty;
    vertexProperty_inner->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_inner->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_inner = new SoTriangleStripSet;
    shape_inner->vertexProperty.setValue( vertexProperty_inner );
    updateInternalShape( state, shape_inner, vertexProperty_inner, fRInner.getValue(), true );
    sep->addChild(shape_inner);

    // add endcaps
    // first endcap, at the beginning of the toroidal segment
    std::cout << "\nBuild starting endcap..." <<std::endl;
    SoVertexProperty* vertexProperty_endcaps_a = new SoVertexProperty;
    vertexProperty_endcaps_a->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_a->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_endcaps_a = new SoTriangleStripSet;
    shape_endcaps_a->vertexProperty.setValue( vertexProperty_endcaps_a );
    buildEndcaps( shape_endcaps_a, vertexProperty_endcaps_a, fRMinor.getValue(), fRInner.getValue(), 0, true);
    sep->addChild(shape_endcaps_a);

    // second endcap, at the end of the toroidal segment
    std::cout << "\nBuild ending endcap..." <<std::endl;
    SoVertexProperty* vertexProperty_endcaps_b = new SoVertexProperty;
    vertexProperty_endcaps_b->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_b->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_endcaps_b = new SoTriangleStripSet;
    shape_endcaps_b->vertexProperty.setValue( vertexProperty_endcaps_b );
    buildEndcaps( shape_endcaps_b, vertexProperty_endcaps_b, fRMinor.getValue(), fRInner.getValue(), m_info.numt );
    sep->addChild(shape_endcaps_b);
  }

  // TODO: maybe I should use a Union at the end, to build a single node with all the parts and return this
  // m_internalShape = sep;
  return sep;
}


//_______________________________________________________
// Build the filled endcap
void
SoTorus::buildEndcaps(SoFaceSet* shape, SoVertexProperty* vertexProperty, double Rxs, int slice, bool invert)
{
  // Each endcap is a disk, made as a FaceSet

  // Number of vertices per strip
  const int verticesPerFace = m_info.numc;

  // Number of minor subdivisions
  const int numStrips = m_info.numc;

  //Set the numVertices field of the single FaceSet accordingly
  shape->numVertices.setValues(0, 1, &verticesPerFace);

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerFace * 1;
  vertexProperty->vertex.setNum( numVerticesTotal );
  vertexProperty->normal.setNum( numVerticesTotal );
  vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = vertexProperty->vertex.startEditing();
  SbVec3f* normals   = vertexProperty->normal.startEditing();
  SbVec2f* texCoords = vertexProperty->texCoord.startEditing();

  // Now fill the buffers
  int vertexIndex = 0;

  // go around cross section
  for ( int strip = 0; strip < numStrips; strip++ )
  {
      vertices[vertexIndex]  = getVertex( Rxs, strip, m_info.numc, slice, m_info.numt );
      normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert );
      texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, slice, m_info.numt );
      vertexIndex++;
  } // end go around cross section

  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();

}

//____________________________________________________
// Build the pierced endcap
void
SoTorus::buildEndcaps(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs, double Rinner, int slice, bool invert)
{
  // Each endcap is a cirular segment, made of one triangleStrip

  // Number of vertices of the strip
  // twice the number of the divisions,
  // plus two additional vertices to close the strip
  const int verticesPerFace = (2 * m_info.numc) + 2;

  // Number of minor subdivisions
  const int numStrips = m_info.numc;

  //Set the numVertices field of the single FaceSet accordingly
  shape->numVertices.setValues(0, 1, &verticesPerFace);

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerFace * 1;
  vertexProperty->vertex.setNum( numVerticesTotal );
  vertexProperty->normal.setNum( numVerticesTotal );
  vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = vertexProperty->vertex.startEditing();
  SbVec3f* normals   = vertexProperty->normal.startEditing();
  SbVec2f* texCoords = vertexProperty->texCoord.startEditing();

  // Now fill the buffers
  int vertexIndex = 0;

  // go around cross section
  for ( int strip = 0; strip < numStrips; strip++ )
  {
      vertices[vertexIndex]  = getVertex( Rxs, strip, m_info.numc, slice, m_info.numt );
      normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert );
      texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, slice, m_info.numt );

      vertexIndex++;

      vertices[vertexIndex]  = getVertex( Rinner, strip, m_info.numc, slice, m_info.numt );
      normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert  );
      texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, slice, m_info.numt );

      vertexIndex++;
  } // end go around cross section

  // last two vertices, to close the strip
  vertices[vertexIndex]  = getVertex( Rxs, 0, m_info.numc, slice, m_info.numt );
  normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert  );
  texCoords[vertexIndex] = getTexCoord( 0, m_info.numc, slice, m_info.numt );

  vertexIndex++;

  vertices[vertexIndex]  = getVertex( Rinner, 0, m_info.numc, slice, m_info.numt );
  normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert  );
  texCoords[vertexIndex] = getTexCoord( 0, m_info.numc, slice, m_info.numt );

  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();

  return;
}





//____________________________________________________________________
void
SoTorus::doAction( SoAction* action )
{
  // Access the state from the action
  SoState* state = action->getState();

  // Do a push before modifying elements as we don't want the shape to leak.
  state->push();

  // Retrieve our implementation scenegraph
  SoNode* internalShape = getInternalShape( state );

  // Here comes the central part of the implementation. We delegate the rendering part to our internal sceneGraph.
  // The forwardTraversal() method enables the action to traverse a sub-scenegraph which is not
  // actually part of the whole inventor sceneGraph. Outside of this node, this sub-scenegraph won't
  // be visible. It won't appear in current path and won't be visible to a search action for instance.
  // action->forwardTraversal( internalShape );
  action->traverse( internalShape );

  // Do a pop after modifying elements as we don't want the shape to leak.
  state->pop();
}


//____________________________________________________________________
bool SoTorus::s_didInit = false;
void SoTorus::initClass()
{
  if ( !s_didInit ) {
    SO_NODE_INIT_CLASS(SoTorus,SoShape,"Shape"); // TODO: check if we can replace "Shape" with something else, e.g., with "Torus".
    s_didInit = true;
  }
}


// generatePrimitives
void SoTorus::generatePrimitives(SoAction *action)
{
  // Redirect action to the doAction() method because
  // all the work is done by the internal shape.
  doAction( action );
}



// Computes vertex position given the current torus subdivision we are working on.
// - "minorSubdiv" is the index of line: for example, strip 0 has vertices on line 0 and 1, strip 1 has them on line 1 and 2, ...
// - "numMinorSubdivs" is the total number of strips around the cross section
// - "subdiv" is the index of the current substrip: e.g., subdiv 0 out of 5 if the strip is divided in 5 substrips from the top view
// - "numSubdivs" is the total number of substrips from the top view
SbVec3f
SoTorus::getVertex( double Rcross, int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs )
{
  // used for the full torus
  // subdiv %= numSubdivs; // when subdiv == numSubdivs it gets back to 0
  // minorSubdiv %= numMinorSubdivs; // TODO: is that needed at all???

  // debug
  // std::cout << "subdiv(stripVertex): " << subdiv << ", minorSubdiv(crossSection): " << minorSubdiv << std::endl;
  // std::cout << "fSPhi: " << fSPhi.getValue() << " - fDPhi: " << fDPhi.getValue() << std::endl;

  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );
  const double minorAngle = TWOPI * static_cast<double>( minorSubdiv ) / static_cast<double>( numMinorSubdivs );

  const double minorAngleCos = fRMajor.getValue() + Rcross * cos(minorAngle); // this is the coordinate along the radius of the torus

  // return the coordinates of the vertex in spherical coordinates
  return SbVec3f( static_cast<float>(minorAngleCos * cos(angle)), // x/y plane
                  static_cast<float>(minorAngleCos * sin(angle)), // x/y plane
                  static_cast<float>(Rcross * sin(minorAngle)) ); // elevation // z
}


// Computes vertex texture coordinates given the current torus subdivision we are working on.
SbVec2f
SoTorus::getTexCoord( int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs )
{
  return SbVec2f( static_cast<float>(minorSubdiv) / static_cast<float>(numMinorSubdivs),
                  1.0f - static_cast<float>(subdiv) / static_cast<float>(numSubdivs) );
}


// Computes vertex normal given the current torus subdivision we are working on.
SbVec3f
SoTorus::getNormal( const SbVec3f& vert, int subdiv, int numSubdivs, bool doInvert )
{
  subdiv %= numSubdivs;

  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  SbVec3f norm( vert[0] - fRMajor.getValue() * static_cast<float>(cos(angle)),
                vert[1] - fRMajor.getValue() * static_cast<float>(sin(angle)),
                vert[2] );
  norm.normalize();
  // if an inner torus, we invert the normals
  if (doInvert) {
    norm.negate();
  }
  return norm;
}


// Computes vertex normal for the endcap
SbVec3f
SoTorus::getNormalEndCap( const SbVec3f& vert, int subdiv, int numSubdivs, bool doInvert )
{
  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  // debug
  // std::cout << "SPhi,DPhi: " << fSPhi.getValue() << "," << fDPhi.getValue() << ", subdiv: " << subdiv << ", numSubdivs: " << numSubdivs << ", angle: " << angle << ", RcosA: " << fRMajor.getValue() * static_cast<float>(cos(angle)) << ", RsinA: " <<  fRMajor.getValue() * static_cast<float>(sin(angle)) << std::endl;
  // std::cout << "vert: " << vert.toString().getString() << std::endl;

  // We want the normal of the endcap's surface to always move outward
  // while following the direction of the major angle,
  // so we build a function for that
  SbVec3f norm;
  if ((angle > M_PI_2) && (angle <= M_PI)) {
    norm.setValue( fRMajor.getValue() * (-1 * static_cast<float>(sin(angle))),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  } else if ((angle > M_PI) && (angle <= 3 * M_PI_2)) {
    norm.setValue( fRMajor.getValue() * abs(static_cast<float>(sin(angle))),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  } else if ((angle > 3 * M_PI_2) && (angle < 2 * M_PI)) {
    norm.setValue( fRMajor.getValue() * abs(static_cast<float>(sin(angle))),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  } else {
    norm.setValue( fRMajor.getValue() * static_cast<float>(sin(angle)),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  }
  norm.normalize();
  // if we chose to invert the normals
  if (doInvert) {
    norm.negate();
  }
  return norm;
}


//
// Fills in numc and numt in the TorusInfo.
//
void
SoTorus::computeComplexityInfo( SoState* state )
{
  // Here we retrieve the complexity value from the state and clamp it between 0.0 and 1.0
  // const float complexity = SbMathHelper::Clamp( SoComplexityElement::get(state), 0.0f, 1.0f );
  const float complexity = Clamp( SoComplexityElement::get(state), 0.0f, 1.0f );

  // Based on the complexity, find the number of verts/normals
  // to use around the cross-section (numc) and torus (numt)
  // Bounding box complexity has already been taken care of by the
  // parent class.
  static const float complexityFactor = 100.0f;
  const float numVerts = complexity * complexityFactor;

  float minorMajorRatio = 0.0f;
  if ( fRMajor.getValue() > 0.0f )
    minorMajorRatio = fRMinor.getValue() / fRMajor.getValue();

  m_info.numt = 3 + static_cast<int>( numVerts );
  m_info.numc = 3 + static_cast<int>( numVerts * minorMajorRatio );

  std::cout << "SoTorus - # faces for the major radius (TorusInfo::numt): " << m_info.numt << std::endl;
  std::cout << "SoTorus - # faces for the minor radius (TorusInfo::numc): " << m_info.numc << std::endl;
}



// getChildren
SoChildList *SoTorus::getChildren() const {
  return m_children;
}


// computeBBox
void SoTorus::computeBBox(SoAction *, SbBox3f &box, SbVec3f &center ){
  RevolutionSurfaceUtil::setBBoxPars(fSPhi.getValue(), fDPhi.getValue(),
				     (fRMajor.getValue() - fRMinor.getValue()),
             (fRMajor.getValue() + fRMinor.getValue()),
				     -fRMinor.getValue(), fRMinor.getValue(),
				     box, center );
}




// updateChildren
void SoTorus::updateChildren() {


  /*
  // Redraw the G4Cons....

  assert(m_children->getLength()==1);
  SoSeparator       *sep                = (SoSeparator *)  ( *m_children)[0];
  SoCoordinate3     *theCoordinates     = (SoCoordinate3 *)      ( sep->getChild(0));
  SoNormal          *theNormals         = (SoNormal *)           ( sep->getChild(1));
  SoNormalBinding   *theNormalBinding   = (SoNormalBinding *)    ( sep->getChild(2));
  SoIndexedFaceSet  *theFaceSet         = (SoIndexedFaceSet *)   ( sep->getChild(3));

  const int NPHI=96, NPOINTS=2*(2*NPHI+2), NFACES=4*NPHI+2, NINDICES = NFACES*5;
  float points[NPOINTS][3], normals[NFACES][3];
#ifdef INVENTOR2_0
  static long     indices[NINDICES];
#else
  static int32_t  indices[NINDICES];
#endif
  static int init=0;
  double phi, pp, DeltaPhi;

  // Indices need to be generated once! This is here to keep it close to the point
  // generation, since otherwise it will be confusing.

  int i;
  if (!init) {
    init = 1;
    // Outer face
    for (i = 0; i< NPHI; i++) {
      // 0 1 3 2;
      indices[5*i+0] =  2*i+0;
      indices[5*i+1] =  2*i+1;
      indices[5*i+2] =  2*i+3;
      indices[5*i+3] =  2*i+2;
      indices[5*i+4] = SO_END_FACE_INDEX;
    }
    // the inner face
    for (i=0;i<NPHI;i++) {
      indices[5*1*NPHI + 5*i+0] = 2*NPHI+2 + 2*i+0;
      indices[5*1*NPHI + 5*i+1] = 2*NPHI+2 + 2*i+1;
      indices[5*1*NPHI + 5*i+2] = 2*NPHI+2 + 2*i+3;
      indices[5*1*NPHI + 5*i+3] = 2*NPHI+2 + 2*i+2;
      indices[5*1*NPHI + 5*i+4] = SO_END_FACE_INDEX;
    }
    // the top side
    for (i=0;i<NPHI;i++) {
      indices[5*2*NPHI + 5*i+0] = 2*i+0;
      indices[5*2*NPHI + 5*i+1] = 2*i+2;
      indices[5*2*NPHI + 5*i+2] = NPOINTS - (2*i+4);
      indices[5*2*NPHI + 5*i+3] = NPOINTS - (2*i+2);
      indices[5*2*NPHI + 5*i+4] = SO_END_FACE_INDEX;
    }
    // the bottom side
    for (i=0;i<NPHI;i++) {
      indices[5*3*NPHI + 5*i+0] = 2*i+1;
      indices[5*3*NPHI + 5*i+1] = NPOINTS - (2*i+1);
      indices[5*3*NPHI + 5*i+2] = NPOINTS - (2*i+3);
      indices[5*3*NPHI + 5*i+3] = 2*i+3;
      indices[5*3*NPHI + 5*i+4] = SO_END_FACE_INDEX;
    }
    // the odd side
    indices[5*4*NPHI +0] = 2*NPHI;
    indices[5*4*NPHI +1] = 2*NPHI+1;
    indices[5*4*NPHI +2] = 2*NPHI+3;
    indices[5*4*NPHI +3] = 2*NPHI+2;
    indices[5*4*NPHI +4] = SO_END_FACE_INDEX;
    // aother odd side
    indices[5*4*NPHI +5 +0] = 0;
    indices[5*4*NPHI +5 +1] = NPOINTS-2;
    indices[5*4*NPHI +5 +2] = NPOINTS-1;
    indices[5*4*NPHI +5 +3] = 1;
    indices[5*4*NPHI +5 +4] = SO_END_FACE_INDEX;
  }
  // Points need to be generated each time:
  // The outer surface
  DeltaPhi = fDPhi.getValue()/NPHI, phi = fSPhi.getValue();
  float  t,st,ct;
  t = FATAN((fRmax2.getValue()-fRmax1.getValue())/(2*fDz.getValue()));
  st = FSIN(t);
  ct = FCOS(t);
  for (i = 0; i<=NPHI; i++) {
    points[2*i+0][0] = fRmax2.getValue()*FCOS(phi);
    points[2*i+0][1] = fRmax2.getValue()*FSIN(phi);
    points[2*i+0][2] = +fDz.getValue();
    points[2*i+1][0] = fRmax1.getValue()*FCOS(phi);
    points[2*i+1][1] = fRmax1.getValue()*FSIN(phi);
    points[2*i+1][2] = -fDz.getValue();
    pp = phi+DeltaPhi/2.0;
    if (i!=NPHI) {
      normals[i][0] = ct * FCOS(pp);
      normals[i][1] = ct * FSIN(pp);
      normals[i][2] = -st;
    }
    phi+=DeltaPhi;
  }
  // The inner surface
  phi = fSPhi.getValue() + fDPhi.getValue();
  t = FATAN((fRmin2.getValue()-fRmin1.getValue())/(2*fDz.getValue()));
  st = FSIN(t);
  ct = FCOS(t);
  for (i = 0; i<=NPHI; i++) {
    points[2*NPHI+2+2*i+0][0] = fRmin2.getValue()*FCOS(phi);
    points[2*NPHI+2+2*i+0][1] = fRmin2.getValue()*FSIN(phi);
    points[2*NPHI+2+2*i+0][2] = +fDz.getValue();
    points[2*NPHI+2+2*i+1][0] = fRmin1.getValue()*FCOS(phi);
    points[2*NPHI+2+2*i+1][1] = fRmin1.getValue()*FSIN(phi);
    points[2*NPHI+2+2*i+1][2] = -fDz.getValue();
    pp = phi-DeltaPhi/2.0;
    if (i!=NPHI) {
      normals[NPHI+i][0] = -ct*FCOS(pp);
      normals[NPHI+i][1] = -ct*FSIN(pp);
      normals[NPHI+i][2] = st;
    }
    phi-=DeltaPhi;
  }
  // The top side
  for (i=0;i<NPHI;i++) {
    normals[2*NPHI+i][0]=normals[2*NPHI+i][1]=0;
    normals[2*NPHI+i][2]=  1.0;
  }
  // The bottom side
  for (i=0;i<NPHI;i++) {
    normals[3*NPHI+i][0]=normals[3*NPHI+i][1]=0;
    normals[3*NPHI+i][2]= -1.0;
  }
  // The odd side
  phi = fSPhi.getValue();
  normals[4*NPHI+0][0]=  FSIN(phi);
  normals[4*NPHI+0][1]= -FCOS(phi);
  normals[4*NPHI+0][2]= 0;

  // Another odd side
  phi = fSPhi.getValue()+fDPhi.getValue();
  normals[4*NPHI+1][0]= -FSIN(phi);
  normals[4*NPHI+1][1]= +FCOS(phi);
  normals[4*NPHI+1][2]=0;

  for (int np=0;np<NPOINTS;np++) theCoordinates->point.set1Value(np,points[np][0],points[np][1],points[np][2]);
  theFaceSet->coordIndex.setValues(0,NINDICES,indices);
  if (smoothDraw.getValue()) {
    //    This Line is replaced by the next one because of an apparent Bug in Inventor (mem. leak).
    //    theNormals->vector.deleteValues(0);
    for (int nf=0;nf<NFACES;nf++) theNormals->vector.set1Value(nf,normals[nf][0],normals[nf][1],normals[nf][2]);
    theNormalBinding->value=SoNormalBinding::PER_FACE;
  }
  else {
    for (int nf=0;nf<NFACES;nf++) theNormals->vector.set1Value(nf,normals[nf][0],normals[nf][1],normals[nf][2]);
    theNormalBinding->value=SoNormalBinding::PER_FACE;
  }

  */

}

// generateChildren
void SoTorus::generateChildren() {

  // TODO: to be updated for Torus...
  /*
  // This routines creates one SoSeparator, one SoCoordinate3, and
  // one SoLineSet, and puts it in the child list.  This is done only
  // once, whereas redrawing the position of the coordinates occurs each
  // time an update is necessary, in the updateChildren routine.

  assert(m_children->getLength() ==0);
  SoSeparator      *sep              = new SoSeparator();
  SoCoordinate3    *theCoordinates   = new SoCoordinate3();
  SoNormal         *theNormals       = new SoNormal();
  SoNormalBinding  *theNormalBinding = new SoNormalBinding();
  SoIndexedFaceSet *theFaceSet       = new SoIndexedFaceSet();
  //
  // This line costs some in render quality! but gives speed.
  //
  sep->addChild(theCoordinates);
  sep->addChild(theNormals);
  sep->addChild(theNormalBinding);
  sep->addChild(theFaceSet);
  m_children->append(sep);
  */
}

// generateAlternateRep
void SoTorus::generateAlternateRep() {

  // This routine sets the alternate representation to the child
  // list of this mode.

  if (m_children->getLength() == 0) generateChildren();
  updateChildren();
  alternateRep.setValue((SoSeparator *)  ( *m_children)[0]);
}

// clearAlternateRep
void SoTorus::clearAlternateRep() {
  alternateRep.setValue(NULL);
}


// #undef SPHERE_NUM_SLICES
// #undef SPHERE_NUM_STACKS
