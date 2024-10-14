/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "VP1GeometrySystems/SoVisualizeAction.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTwistedTrap.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoTessellatedSolid.h"
#include "GeoModelKernel/GeoFacet.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GXHepVis/nodes/SoTubs.h"
#include "GXHepVis/nodes/SoCons.h"
#include "GXHepVis/nodes/SoTorus.h"
#include "GXHepVis/nodes/SoGenericBox.h"
#include "GXHepVis/nodes/SoPcons.h"
#include "GXHepVis/nodes/SoTessellated.h"
#include "GXHepVis/SbPolyhedron.h"
#include "GXHepVis/nodes/SoLAr.h"
#include "GXHepVis/nodes/SoPolyhedron.h"
#include "GXHepVis/GXHepVisUtils.h"
#include "VP1GeometrySystems/SbPolyhedrizeAction.h"
#include <Inventor/nodes/SoSphere.h>

#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/GeoTrapezoidSurface.h"
#include "GeoModelKernel/GeoAnnulusSurface.h"
#include "GeoModelKernel/GeoDiamondSurface.h"
// System of units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // --> 'GeoModelKernelUnits::cm'
#include <iostream>
#include <cmath>

SoVisualizeAction::SoVisualizeAction()
  : m_shape(0)
{
  GXHepVisUtils::initAllCustomClasses();
  setDepthLimit(0);
}

SoVisualizeAction::~SoVisualizeAction()
{
  // NOTE: Don't delete.  Let ref count take care of the memory.
}


void SoVisualizeAction::handleShape(const GeoShape *shape)
{
  // We don't recognize it.  Try to polyhedrize it!
  SbPolyhedrizeAction a;
  shape->exec(&a);
  const SbPolyhedron *poly =a.getPolyhedron();
  if (poly) {
    SoPolyhedron *myPoly = new SoPolyhedron(poly);
    m_shape=myPoly;
  }

}

void SoVisualizeAction::handleBox(const GeoBox *box)
{
  SoGenericBox * gb = new SoGenericBox;
  gb->setParametersForBox( box->getXHalfLength(),box->getYHalfLength(),box->getZHalfLength() );
  m_shape=gb;
}

void SoVisualizeAction::handleCons(const GeoCons *cons)
{
  SoCons::initClass();
  SoCons *socons= new SoCons;
  socons->fRmin1 =cons->getRMin1();
  socons->fRmin2 =cons->getRMin2();
  socons->fRmax1 =cons->getRMax1();
  socons->fRmax2 =cons->getRMax2();
  socons->fDz    =cons->getDZ();
  socons->fSPhi  =cons->getSPhi();
  socons->fDPhi  =cons->getDPhi();

  m_shape=socons;
}

void SoVisualizeAction::handleTorus(const GeoTorus *torus)
{
  SoTorus::initClass();
  SoTorus *sotorus= new SoTorus;
  sotorus->fRInner = torus->getRMin();
  sotorus->fRMinor = torus->getRMax();
  sotorus->fRMajor = torus->getRTor();
  sotorus->fSPhi = torus->getSPhi();
  sotorus->fDPhi = torus->getDPhi();

  m_shape=sotorus;
}

void SoVisualizeAction::handlePcon(const GeoPcon *pcon)
{
  //Set up temporary data arrays for profile:
  float *z  = new float[pcon->getNPlanes()];
  float *rmn= new float[pcon->getNPlanes()];
  float *rmx= new float[pcon->getNPlanes()];

  for (unsigned s=0;s<pcon->getNPlanes();++s) {
    z[s]=pcon->getZPlane(s);
    rmn[s]=pcon->getRMinPlane(s);
    rmx[s]=pcon->getRMaxPlane(s);
  }

  SoPcons::initClass();
  SoPcons *sopcons = new SoPcons();
  sopcons->fSPhi = pcon->getSPhi();
  sopcons->fDPhi = pcon->getDPhi();
  sopcons->fRmin.setValues(0,pcon->getNPlanes(),rmn);
  sopcons->fRmax.setValues(0,pcon->getNPlanes(),rmx);
  sopcons->fDz.setValues  (0,pcon->getNPlanes(),z);

  //Delete temporary arrays:
  delete  [] z;
  delete  [] rmn;
  delete  [] rmx;

  m_shape=sopcons;

}

void SoVisualizeAction::handleTrap(const GeoTrap *trap)
{
  SoGenericBox * gb = new SoGenericBox;
  gb->setParametersForTrapezoid(trap->getZHalfLength(), trap->getTheta(), trap->getPhi(),
				trap->getDydzn(), trap->getDxdyndzn(), trap->getDxdypdzn(),
				trap->getDydzp(), trap->getDxdyndzp(), trap->getDxdypdzp(),
				trap->getAngleydzn(), trap->getAngleydzp());
  m_shape=gb;
}

void SoVisualizeAction::handleTwistedTrap(const GeoTwistedTrap *twistedtrap)
{
    SbPolyhedrizeAction a;
    twistedtrap->exec(&a);
    const SbPolyhedron *poly =a.getPolyhedron();
    if (poly) {
      SoPolyhedron *myPoly = new SoPolyhedron(poly);
      m_shape=myPoly;
    }
}

void SoVisualizeAction::handleTrd(const GeoTrd *trd)
{
  SoGenericBox * gb = new SoGenericBox;
  gb->setParametersForTrd( trd->getXHalfLength1(), trd->getXHalfLength2(),
			   trd->getYHalfLength1(), trd->getYHalfLength2(),
			   trd->getZHalfLength() );
  m_shape=gb;
}

void SoVisualizeAction::handleTube(const GeoTube *tube)
{
  SoTubs *sotubs= new SoTubs;
  sotubs->pRMin= tube->getRMin();
  sotubs->pRMax= tube->getRMax();
  sotubs->pDz  = tube->getZHalfLength();
  sotubs->pSPhi= 0;
  sotubs->pDPhi= 2*M_PI;
  m_shape=sotubs;
}

void SoVisualizeAction::handleTubs(const GeoTubs *tubs)
{
  SoTubs *sotubs= new SoTubs;
  sotubs->pRMin= tubs->getRMin();
  sotubs->pRMax= tubs->getRMax();
  sotubs->pDz  = tubs->getZHalfLength();
  sotubs->pSPhi= tubs->getSPhi();
  sotubs->pDPhi= tubs->getDPhi();
  m_shape=sotubs;
}

void SoVisualizeAction::handleSimplePolygonBrep(const GeoSimplePolygonBrep *brep)
{
  //Fixme: Detect if order of vertices is the wrong way around... and reorder if necessary.
  double dz = brep->getDZ();
  std::vector<double> x, y;
  for(unsigned int i=0; i<brep->getNVertices(); ++i)
  {
    x.push_back(brep->getXVertex(i));
    y.push_back(brep->getYVertex(i));
  }

  SbPolyhedronPolygonXSect sbPoly(x,y,dz);
  SoPolyhedron* soPoly = new SoPolyhedron(sbPoly);
  m_shape = soPoly;
}

void SoVisualizeAction::handleTessellatedSolid (const GeoTessellatedSolid* geoTessellated)
{
  SoTessellated * soTessellated = new SoTessellated;

  //std::cout << "\tn. facets: " << geoTessellated->getNumberOfFacets() << std::endl;
  for(size_t i=0; i<geoTessellated->getNumberOfFacets();++i) {
    GeoFacet* facet = geoTessellated->getFacet(i);
    if(facet->getNumberOfVertices()==3) {
    	//std::cout << "\tfacet " << i << " is a TRI - vertexType: " << facet->getVertexType()
    	//		<< " - " << facet->getVertex(0).x() << "," << facet->getVertex(0).y() << "," << facet->getVertex(0).z()
    	//		<< " - " << facet->getVertex(1).x() << "," << facet->getVertex(1).y() << "," << facet->getVertex(1).z()
    	//		<< " - " << facet->getVertex(2).x() << "," << facet->getVertex(2).y() << "," << facet->getVertex(2).z()
	//			<< std::endl;
      if(facet->getVertexType()==GeoFacet::ABSOLUTE) {
	soTessellated->addTriangularFacet(facet->getVertex(0).x(),facet->getVertex(0).y(),facet->getVertex(0).z(),
					  facet->getVertex(1).x(),facet->getVertex(1).y(),facet->getVertex(1).z(),
					  facet->getVertex(2).x(),facet->getVertex(2).y(),facet->getVertex(2).z());
      }  else {
	soTessellated->addTriangularFacet(facet->getVertex(0).x(),facet->getVertex(0).y(),facet->getVertex(0).z(),
					  facet->getVertex(0).x()+facet->getVertex(1).x(),
					  facet->getVertex(0).y()+facet->getVertex(1).y(),
					  facet->getVertex(0).z()+facet->getVertex(1).z(),
					  facet->getVertex(0).x()+facet->getVertex(2).x(),
					  facet->getVertex(0).y()+facet->getVertex(2).y(),
					  facet->getVertex(0).z()+facet->getVertex(2).z());
    }
    } // end of "TRI"
    else {
    	//std::cout << "\tfacet " << i << " is a QUAD - vertexType: " << facet->getVertexType()
    	//		<< " - " << facet->getVertex(0).x() << "," << facet->getVertex(0).y() << "," << facet->getVertex(0).z()
    	//		<< " - " << facet->getVertex(1).x() << "," << facet->getVertex(1).y() << "," << facet->getVertex(1).z()
    	//		<< " - " << facet->getVertex(2).x() << "," << facet->getVertex(2).y() << "," << facet->getVertex(2).z()
    	//		<< " - " << facet->getVertex(3).x() << "," << facet->getVertex(3).y() << "," << facet->getVertex(3).z()
	//			<< std::endl;
      if(facet->getVertexType()==GeoFacet::ABSOLUTE) {
	soTessellated->addQuadrangularFacet(facet->getVertex(0).x(),facet->getVertex(0).y(),facet->getVertex(0).z(),
					    facet->getVertex(1).x(),facet->getVertex(1).y(),facet->getVertex(1).z(),
					    facet->getVertex(2).x(),facet->getVertex(2).y(),facet->getVertex(2).z(),
					    facet->getVertex(3).x(),facet->getVertex(3).y(),facet->getVertex(3).z());
      } else {
	soTessellated->addQuadrangularFacet(facet->getVertex(0).x(),facet->getVertex(0).y(),facet->getVertex(0).z(),
					    facet->getVertex(0).x()+facet->getVertex(1).x(),
					    facet->getVertex(0).y()+facet->getVertex(1).y(),
					    facet->getVertex(0).z()+facet->getVertex(1).z(),
					    facet->getVertex(0).x()+facet->getVertex(2).x(),
					    facet->getVertex(0).y()+facet->getVertex(2).y(),
					    facet->getVertex(0).z()+facet->getVertex(2).z(),
					    facet->getVertex(0).x()+facet->getVertex(3).x(),
					    facet->getVertex(0).y()+facet->getVertex(3).y(),
					    facet->getVertex(0).z()+facet->getVertex(3).z());

    }
    } // end of "QUAD"
  }
  soTessellated->finalize();
  m_shape=soTessellated;
}

void SoVisualizeAction::handleGenericTrap(const GeoGenericTrap *gentrap)
{
  SoGenericBox * gb = new SoGenericBox;
  const GeoGenericTrapVertices& trapVertices = gentrap->getVertices();
  double dZ = gentrap->getZHalfLength();
  gb->setGenericParameters(trapVertices[7].x(),trapVertices[7].y(),-dZ,
			   trapVertices[6].x(),trapVertices[6].y(),-dZ,
			   trapVertices[5].x(),trapVertices[5].y(),-dZ,
			   trapVertices[4].x(),trapVertices[4].y(),-dZ,
			   trapVertices[3].x(),trapVertices[3].y(),dZ,
			   trapVertices[2].x(),trapVertices[2].y(),dZ,
			   trapVertices[1].x(),trapVertices[1].y(),dZ,
			   trapVertices[0].x(),trapVertices[0].y(),dZ);
  m_shape=gb;
}

// In the near future, I should separate those handle VS methods from SoVisualizationAction

void SoVisualizeAction::handleRectSurface(const GeoRectSurface *surf)
{
  static float norms[2][3]={
    {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}
  };
  surfaceNorms->vector.setValues(0, 2, norms);
  surfBinding->value = SoNormalBinding::PER_FACE;
  surfMaterialBinding->value = SoMaterialBinding::PER_FACE;
  
  float dx = surf->getXHalfLength();
  float dy = surf->getYHalfLength();

  float vertices[8][3]={
  {dx,dy,0}, {-dx,dy,0},{-dx,-dy,0}, {dx,-dy,0},
  {dx,dy,0}, {-dx,dy,0},{-dx,-dy,0}, {dx,-dy,0}
  };
  SoCoordinate3 *coords_ = new SoCoordinate3;
  coords_->point.setValues(0,8, vertices);
  coords = coords_;
  
  int32_t numvertices[2] = {4, 4};
  SoFaceSet * faceset = new SoFaceSet;
  faceset->numVertices.setValues(0, 2, numvertices);
  m_shape = faceset;
  
  vs_shape->addChild(surfaceNorms);
  vs_shape->addChild(surfBinding);
  vs_shape->addChild(surfMaterialBinding);
  vs_shape->addChild(coords);
  vs_shape->addChild(m_shape);

}

void SoVisualizeAction::handleTrapezoidSurface(const GeoTrapezoidSurface *surf)
{
  static float norms[2][3]={
    {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}
  };
  surfaceNorms->vector.setValues(0, 2, norms);
  surfBinding->value = SoNormalBinding::PER_FACE;
  surfMaterialBinding->value = SoMaterialBinding::PER_FACE;
  
  float dx_min = surf->getXHalfLengthMin();
  float dx_max = surf->getXHalfLengthMax();
  float dy = surf->getYHalfLength();

  float vertices[8][3]={
  {dx_max,dy,0}, {-dx_max,dy,0},{-dx_min,-dy,0}, {dx_min,-dy,0},
  {dx_max,dy,0}, {-dx_max,dy,0},{-dx_min,-dy,0}, {dx_min,-dy,0}
  };
  SoCoordinate3 *coords_ = new SoCoordinate3;
  coords_->point.setValues(0,8, vertices);
  coords = coords_;
  
  int32_t numvertices[2] = {4, 4};
  SoFaceSet * faceset = new SoFaceSet;
  faceset->numVertices.setValues(0, 2, numvertices);
  m_shape = faceset;

  vs_shape->addChild(surfaceNorms);
  vs_shape->addChild(surfBinding);
  vs_shape->addChild(surfMaterialBinding);
  vs_shape->addChild(coords);
  vs_shape->addChild(m_shape);
}

// Here I'm working on AnnulusSurface, also, this part should be separated from SoVisualizationAction

// use static keyword to make the function private
static int initialize_N(double theta){
  int N;

  if (theta <= 0.0){
    throw std::runtime_error("Theta cannot be zero or negative");
  }
  else{
    double phi = 2.0*M_PI;
    double angle = fmod(theta, phi);    // make sure that angle is between 0 and 2*pi
    if (angle != 0.0){
      N = 4*std::ceil(360*angle/phi); // 360 is the number of rectangles in the circle
                                      // 4 is the number of vertices in each rectangle
    }
    else{
      N = 360;
    }
  }                                    
  return N;
}

static int which_quadrant(double theta){
  double phi = 2.0*M_PI;
  double angle = fmod(theta, phi);
  if (angle < phi/4.0){
    return 0;
  } else if (angle < 2.0*phi/4.0){
    return 1;
  } else if (angle < 3.0*phi/4.0){
    return 2;
  } else {
    return 3;
  }
}

static double solve_X(double k, double Ox, double Oy, double radius, double theta){
  if(std::isinf(k) || k > 1e5 || k < -1e5){
    return Ox;
  }
  else{
    double a = 1.0 + k*k;
    double b = 2*k*Oy - 2*k*k*Ox;
    double c = k*k*Ox*Ox - 2*k*Ox*Oy + Oy*Oy - radius*radius;
    double delta = b*b - 4*a*c;

    if (delta < 0){
      throw std::runtime_error("No solution for X");
    }

    double X1 = (-b + sqrt(delta))/(2*a);
    double X2 = (-b - sqrt(delta))/(2*a);

    int quadrant = which_quadrant(theta);
    if (quadrant == 0 || quadrant == 3){
      return X1;
    } else {
      return X2;
    }
  }
}

static double solve_Y(double X, double k, double Ox, double Oy, double radius, double theta){
  if(std::isinf(k)){
    int quadrant = which_quadrant(theta);
    if (quadrant == 0 || quadrant == 1){
      return sqrt(radius*radius - X*X);
    } else {
      return -sqrt(radius*radius - X*X);
    }
  }
  else if (k > 1e5 || k < -1e5){
    int quadrant = which_quadrant(theta);
    if (quadrant == 0 || quadrant == 1){
      return sqrt(radius*radius - X*X);
    } else {
      return -sqrt(radius*radius - X*X);
    }
  }
  else{
    return k*X + Oy - k*Ox;
  }
}

static void tiny_rect( double Ox, double Oy, double radius_in, double radius_out, double angle, double delta_angle, float **rect_vertices ){

  double X1, X2, X3, X4, Y1, Y2, Y3, Y4;
  double k = tan(angle);
  double k_plus = tan(angle + delta_angle);

  X1 = solve_X(k, Ox, Oy, radius_out, angle);
  Y1 = solve_Y(X1, k, Ox, Oy, radius_out, angle);

  X2 = solve_X(k_plus, Ox, Oy, radius_out, angle + delta_angle);
  Y2 = solve_Y(X2, k_plus, Ox, Oy, radius_out, angle + delta_angle);

  X3 = solve_X(k_plus, Ox, Oy, radius_in, angle + delta_angle);
  Y3 = solve_Y(X3, k_plus, Ox, Oy, radius_in, angle + delta_angle);

  X4 = solve_X(k, Ox, Oy, radius_in, angle);
  Y4 = solve_Y(X4, k, Ox, Oy, radius_in, angle);

  // generate vertices for the tiny rectangle
  rect_vertices[0][0] = static_cast<float>(X1);
  rect_vertices[0][1] = static_cast<float>(Y1);
  rect_vertices[0][2] = 0.0f;
  
  rect_vertices[1][0] = static_cast<float>(X2);
  rect_vertices[1][1] = static_cast<float>(Y2);
  rect_vertices[1][2] = 0.0f;
  
  rect_vertices[2][0] = static_cast<float>(X3);
  rect_vertices[2][1] = static_cast<float>(Y3);
  rect_vertices[2][2] = 0.0f;

  rect_vertices[3][0] = static_cast<float>(X4);
  rect_vertices[3][1] = static_cast<float>(Y4);
  rect_vertices[3][2] = 0.0f;
}

void SoVisualizeAction::handleAnnulusSurface(const GeoAnnulusSurface *surf)
{

  double real_theta = surf->getPhi();
  double Ox = surf->getOx();
  double Oy = surf->getOy();
  double radius_in = surf->getRadiusIn();
  double radius_out = surf->getRadiusOut();
  double angle;  
  int n = initialize_N(real_theta);  // initialize the number of rectangles, also make sure that real theta is positive

  int quotient = floor(real_theta/(2.0*M_PI));
  double theta = real_theta - quotient*2.0*M_PI;
  if (theta == 0.0){
    theta = 2.0*M_PI;
  }  
  double delta_angle = theta/n;
  
  // size of rect_vertices is 4x3
  float **rect_vertices = new float*[4];
  for (int i = 0; i < 4; ++i) {
      rect_vertices[i] = new float[3];
  }

  float vertices[8][3];
  
  // add material (fake material, for drawing purposes) bindings to the surface
  surfMaterialBinding->value = SoMaterialBinding::PER_FACE;
  vs_shape->addChild(surfMaterialBinding);
  
  // set the surface normal properties
  static float norms[2][3]={
    {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}
  };
  surfaceNorms->vector.setValues(0, 2, norms);
  surfBinding->value = SoNormalBinding::PER_FACE;
  vs_shape->addChild(surfaceNorms);
  vs_shape->addChild(surfBinding);
  SoCoordinate3 *coords = new SoCoordinate3;
  // Add coordinates & faceSet child for each rectangle
  for(int i = 0; i < n; i++){
    angle = 0.0 + i*delta_angle;    
    tiny_rect(Ox, Oy, radius_in, radius_out, angle, delta_angle, rect_vertices);

    SoCoordinate3 *coords = new SoCoordinate3;
    SoFaceSet     *faceSet= new SoFaceSet;

    for (int i = 0; i < 2; i++){
      for (int j = 0; j < 4; j++){
        for (int k = 0; k < 3; k++){
          vertices[i*4 + j][k] = rect_vertices[j][k];
        }
      }
    }
    coords->point.setValues(0, 8, vertices); // front side + back side = 8 vertices
    int32_t numvertices[2] = {4, 4};
    faceSet->numVertices.setValues(0, 2, numvertices);
    vs_shape->addChild(coords);
    vs_shape->addChild(faceSet);
  }
}

void SoVisualizeAction::handleDiamondSurface(const GeoDiamondSurface *surf)
{
  static float norms[2][3]={
    {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}
  };
  surfaceNorms->vector.setValues(0, 2, norms);
  surfBinding->value = SoNormalBinding::PER_FACE;
  surfMaterialBinding->value = SoMaterialBinding::PER_FACE;
  
  float dx1 = surf->getXbottomHalf();
  float dx2 = surf->getXmidHalf();
  float dx3 = surf->getXtopHalf();
  float dy1 = surf->getYbottomHalf();
  float dy2 = surf->getYtopHalf();

  float vertices[12][3]={
  {dx3,dy2,0}, {-dx3,dy2,0},{-dx2,0,0}, {-dx1,-dy1,0}, {dx1,-dy1,0}, {dx2,0,0},
  {dx3,dy2,0}, {-dx3,dy2,0},{-dx2,0,0}, {-dx1,-dy1,0}, {dx1,-dy1,0}, {dx2,0,0}
  };
  SoCoordinate3 *coords_ = new SoCoordinate3;
  coords_->point.setValues(0,12, vertices);
  coords = coords_;
  
  int32_t numvertices[2] = {6, 6};
  SoFaceSet * faceset = new SoFaceSet;
  faceset->numVertices.setValues(0, 2, numvertices);
  m_shape = faceset;

  vs_shape->addChild(surfaceNorms);
  vs_shape->addChild(surfBinding);
  vs_shape->addChild(surfMaterialBinding);
  vs_shape->addChild(coords);
  vs_shape->addChild(m_shape);
}