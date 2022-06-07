/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// Minimal Plugin
// Riccardo Maria BIANCHI, 2022 May 6 
//
// This is an example plugin. It compiles to a shared library 
// (with .so or .dylib extension) which can be viewed with gmex.
// In this example there is no "envelope", all the components of
// the minimal example geometry are placed into the world.  
//
// This example illustrates the use of:
//
//    --the plugin mechanism.
//    --simple shapes, logical volumes, physical volumes
//    --chemical elements and pure materials (aluminium)
//
// --------------------------------------------------------------------

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"

// Class Declaration

class MinimalPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  MinimalPlugin();

  // Destructor:
  ~MinimalPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const MinimalPlugin & operator=(const MinimalPlugin &right)=delete;
  MinimalPlugin(const MinimalPlugin &right) = delete;

};


// Class definition:

// Constructor
MinimalPlugin::MinimalPlugin()
{
}

// Destructor
MinimalPlugin::~MinimalPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void MinimalPlugin::create(GeoPhysVol *world, bool /*publish*/)
{
  const double degree=M_PI/180.0;

  // Define elements used in this example:
  GeoElement  *aluminium     = new GeoElement("Aluminium", "Al", 13,  26 * GeoModelKernelUnits::g/GeoModelKernelUnits::mole);

  // Define materials:
  // Define Aluminium
  double densityOfAluminium=2.7;                  // g/cm^3
  GeoMaterial *Aluminium     = new GeoMaterial("Aluminium", densityOfAluminium);
  Aluminium->add(aluminium,1.0);
  Aluminium->lock();

  // Some dimensions used below:
  double platformHeight=34.5;                             // Height to the top of the flanges
  //double flangeDiameter=3.375;                            // Diameter of the flanges
  double flangeThickness=3.0/16.0;                        // Thickness of the flanges
  double t1TubeLength = platformHeight-flangeThickness;   // Overall length of tube t1;
  double innerRadius=0.75/2.0;                            // 3/4 inch (inner diameter) pipe
  double outerRadius=(17.0/16.0)/2.0;                     // 1-1/16   (outer diameter) 
  double leftRightLegSeparation=61.0+11.0/16.0;           // Distance between legs, in x. 
  double frontBackLegSeparation=19.0+3.0/4.0;             // Distance between legs, in y.
  //const double barWidth1=4.0;                             // Width of front-back primary support
  //const double barWidth2=3.0;                             // Width of secondary aluminium support
  //double barThickness   =1.25;                            // Thickness of Aluminium bars
  //double cutoutDepth    = 21.5;                           // Depth of the cutout hole
  //double cutoutWidth    = 32.375;                         // Width of the cutout hole

  // Add the four legs of the kitchen sink:
  {
    const GeoTube      *t1Tube    = new GeoTube(innerRadius,outerRadius, t1TubeLength/2.0);
    const GeoLogVol    *t1Log     = new  GeoLogVol("T1Log", t1Tube, Aluminium);
    const GeoLogVol    *t2Log     = new  GeoLogVol("T2Log", t1Tube, Aluminium);
    GeoPhysVol         *t1Phys    = new GeoPhysVol(t1Log);
    GeoPhysVol         *t2Phys    = new GeoPhysVol(t2Log);
    //GeoPhysVol         *t3Phys    = new GeoPhysVol(t1Log);
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(leftRightLegSeparation/2.0, frontBackLegSeparation/2.0, 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(leftRightLegSeparation/2.0, -frontBackLegSeparation/2.0, 0));
    GeoTransform  *xform3         = new GeoTransform(GeoTrf::Translate3D(-leftRightLegSeparation/2.0, -frontBackLegSeparation/2.0, 0));
    //GeoTransform  *xform4         = new GeoTransform(GeoTrf::Translate3D(-leftRightLegSeparation/2.0, frontBackLegSeparation/2.0, 0));
    world->add(xform1);
    
    world->add(t1Phys);
    
    world->add(xform2);
    world->add(t2Phys);
    
    world->add(xform3); 
    world->add(t2Phys);
    world->add(t2Phys);
    world->add(t2Phys);

  }
    
  
  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" MinimalPlugin *createMinimalPlugin() {
  return new MinimalPlugin;
}
