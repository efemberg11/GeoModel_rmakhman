/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// Kitchen Sink Plugin
// Joe Boudreau Jan 11 2021
//
// This is an example plugin. It compiles to a shared library 
// (with .so or .dylib extension) which can be viewed with gmex.
// In this example there is no "envelope", all the components of
// the kitchen sink are placed into the world.  This example
// contains a geometry clash or two.  Therefore it may also be
// used to test clash detection.
//
// This example illustrates the use of:
//
//    --the plugin mechanism.
//    --simple shapes, logical volumes, physical volumes
//    --Boolean operations
//    --define air 
//    --pure materials (aluminium)
//    --mixtures (stainless steel)
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
#include "GeoModelKernel/Units.h"
using namespace GeoModelKernelUnits;

// Class Declaration

class KitchenSinkPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  KitchenSinkPlugin();

  // Destructor:
  ~KitchenSinkPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const KitchenSinkPlugin & operator=(const KitchenSinkPlugin &right)=delete;
  KitchenSinkPlugin(const KitchenSinkPlugin &right) = delete;

};


// Class definition:

// Constructor
KitchenSinkPlugin::KitchenSinkPlugin()
{
}

// Destructor
KitchenSinkPlugin::~KitchenSinkPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void KitchenSinkPlugin::create(GeoPhysVol *world, bool /*publish*/)
{
  const double degree=M_PI/180.0;

  // Define elements used in this example:
  GeoElement  *oxygen        = new GeoElement("Oxygen",    "O",   19,  39*gram/mole);
  GeoElement  *nitrogen      = new GeoElement("Nitrogen",  "N",   7,  14*gram/mole);
  GeoElement  *argon         = new GeoElement("Argon",     "Ar", 18,  40*gram/mole);
  GeoElement  *aluminium     = new GeoElement("Aluminium", "Al", 13,  26*gram/mole);
  GeoElement  *iron          = new GeoElement("Iron",      "Fe", 26,  55.8*gram/mole);
  GeoElement  *chromium      = new GeoElement("Chromium",  "Cr", 24,  52*gram/mole);

  // Define materials:
  
  // Define Air:
  double densityOfAir     = 1.2E-3*gram/cm3;               // g/cm^3
  GeoMaterial *Air           = new GeoMaterial("Air",densityOfAir);
  Air->add(oxygen,2*0.21);                        // diatomic   oxygen   21% by volume.
  Air->add(nitrogen,2*0.78);                        // diatomic   nitrogen 78% by volume.
  Air->add(argon,0.01);                          // monoatomic argon    78% by volume.
  Air->lock();

  // Define Aluminium
  double densityOfAluminium=2.7*gram/cm3;                  // g/cm^3
  GeoMaterial *Aluminium     = new GeoMaterial("Aluminium", densityOfAluminium);
  Aluminium->add(aluminium,1.0);
  Aluminium->lock();

  // Define Iron
  double densityOfIron=7.9*gram/cm3;                       // g/cm^3
  GeoMaterial *Iron           = new GeoMaterial("Iron", densityOfIron);
  Iron->add(iron,1.0);
  Iron->lock();

  // Define Stainless Steel ("Stainless")
  double densityOfStainless=7.9*gram/cm3;                  // g/cm^3
  GeoMaterial *Stainless       = new GeoMaterial("Stainless", densityOfStainless);
  Stainless->add(iron,0.89);
  Stainless->add(chromium, 0.11);
  Stainless->lock();

  // Some dimensions used below:
  double platformHeight=34.5;                             // Height to the top of the flanges
  double flangeDiameter=3.375;                            // Diameter of the flanges
  double flangeThickness=3.0/16.0;                        // Thickness of the flanges
  double t1TubeLength = platformHeight-flangeThickness;   // Overall length of tube t1;
  double innerRadius=0.75/2.0;                            // 3/4 inch (inner diameter) pipe
  double outerRadius=(17.0/16.0)/2.0;                     // 1-1/16   (outer diameter) 
  double leftRightLegSeparation=61.0+11.0/16.0;           // Distance between legs, in x. 
  double frontBackLegSeparation=19.0+3.0/4.0;             // Distance between legs, in y.
  const double barWidth1=4.0;                             // Width of front-back primary support
  const double barWidth2=3.0;                             // Width of secondary aluminium support
  double barThickness   =1.25;                            // Thickness of Aluminium bars
  double cutoutDepth    = 21.5;                           // Depth of the cutout hole
  double cutoutWidth    = 32.375;                         // Width of the cutout hole

  // Add the four legs of the kitchen sink:
  {
    const GeoTube      *t1Tube    = new GeoTube(innerRadius,outerRadius, t1TubeLength/2.0);
    const GeoLogVol    *t1Log     = new  GeoLogVol("T1Log", t1Tube, Iron);
    GeoPhysVol         *t1Phys    = new GeoPhysVol(t1Log);
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(leftRightLegSeparation/2.0, frontBackLegSeparation/2.0, 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(leftRightLegSeparation/2.0, -frontBackLegSeparation/2.0, 0));
    GeoTransform  *xform3         = new GeoTransform(GeoTrf::Translate3D(-leftRightLegSeparation/2.0, -frontBackLegSeparation/2.0, 0));
    GeoTransform  *xform4         = new GeoTransform(GeoTrf::Translate3D(-leftRightLegSeparation/2.0, frontBackLegSeparation/2.0, 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    world->add(xform3);
    world->add(t1Phys);
    world->add(xform4); 
    world->add(t1Phys);
  }
    
  // Add two crossbar tubes
  {
    const GeoTube      *t2Tube  = new GeoTube(innerRadius, outerRadius,frontBackLegSeparation/2.0);
    const GeoLogVol    *t2Log   = new  GeoLogVol("T2Log", t2Tube, Iron);
    GeoPhysVol         *t2Phys = new GeoPhysVol(t2Log);
    for (int i=0;i<2;i++) 
      {
	GeoTransform       *t2Transform = new GeoTransform(
							   
							   GeoTrf::RotateX3D(90.0*degree)*
							   GeoTrf::TranslateX3D((1-2.0*i)*leftRightLegSeparation/2.0)
							   );
	world->add(t2Transform);
	world->add(t2Phys);
      }
  }
  // Add the long cross tube:
  {
    const GeoTube      *t3Tube  = new GeoTube(innerRadius, outerRadius,leftRightLegSeparation/2.0);
    const GeoLogVol    *t3Log   = new  GeoLogVol("T3Log", t3Tube, Iron);
    GeoPhysVol         *t3Phys  = new GeoPhysVol(t3Log);
    GeoTransform       *t3Transform = new GeoTransform(GeoTrf::RotateY3D(90.0*degree));
    
    world->add(t3Transform);
    world->add(t3Phys);
  }
  
  // Add the flanges:
  {
    const GeoTube      *flTube  = new GeoTube(0, flangeDiameter/2.0,flangeThickness/2.0);
    const GeoLogVol    *flLog   = new  GeoLogVol("FlLog", flTube, Iron);
    GeoPhysVol         *flPhys  = new GeoPhysVol(flLog);
    double height=(t1TubeLength+flangeThickness)/2.0;
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(leftRightLegSeparation/2.0, frontBackLegSeparation/2.0, height));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(leftRightLegSeparation/2.0, -frontBackLegSeparation/2.0, height));
    GeoTransform  *xform3         = new GeoTransform(GeoTrf::Translate3D(-leftRightLegSeparation/2.0, -frontBackLegSeparation/2.0, height));
    GeoTransform  *xform4         = new GeoTransform(GeoTrf::Translate3D(-leftRightLegSeparation/2.0, frontBackLegSeparation/2.0, height));
    world->add(xform1);
    world->add(flPhys);
    world->add(xform2);
    world->add(flPhys);
    world->add(xform3);
    world->add(flPhys);
    world->add(xform4);
    world->add(flPhys);

  }  
  // Add the primary front-back supports
  {
    const GeoBox       *b1Box  = new GeoBox(barWidth1/2.0, barThickness/2.0,(cutoutDepth+2*barWidth2)/2.0);
    const GeoLogVol    *b1Log  = new GeoLogVol("B1Log", b1Box, Aluminium);
    GeoPhysVol         *b1Phys = new GeoPhysVol(b1Log);
    for (int i=0;i<2;i++)  {
      double height=platformHeight/2.0+barThickness/2.0+flangeThickness/2.0;
      GeoTransform       *t2Transform = new GeoTransform(
							 
							 GeoTrf::RotateX3D(90.0*degree)*
							 GeoTrf::Translate3D((1-2.0*i)*leftRightLegSeparation/2.0,height,0)
							 );
      world->add(t2Transform);
      world->add(b1Phys);
    }
  }
  // Add the secondary bars running left to right
  {
    const GeoBox       *b2Box  = new GeoBox((leftRightLegSeparation+barWidth1)/2.0, barWidth2/2.0,barThickness/2.0);
    const GeoLogVol    *b2Log  = new  GeoLogVol("B2Log", b2Box, Aluminium);
    GeoPhysVol         *b2Phys = new GeoPhysVol(b2Log);
    for (int i=0;i<2;i++) {
      double height=platformHeight/2.0+barThickness/2.0+flangeThickness/2.0+barThickness;
      GeoTransform       *b2Transform = new GeoTransform(
							 GeoTrf::Translate3D(0,(1-2.0*i)*(cutoutDepth+barWidth2)/2.0,height)
							 );
      world->add(b2Transform);
      world->add(b2Phys);
    }
  }
  // Add the end "filler" bars:
  {
    const GeoBox       *b3Box  = new GeoBox(barWidth1/2.0, cutoutDepth/2.0,barThickness/2.0);
    const GeoLogVol    *b3Log  = new GeoLogVol("B3Log", b3Box, Aluminium);
    GeoPhysVol         *b3Phys = new GeoPhysVol(b3Log);
    double xDist[]={leftRightLegSeparation/2.0,-leftRightLegSeparation/2.0};
    for (int i=0;i<2;i++) {
      double height=platformHeight/2.0+3.0*barThickness/2.0+flangeThickness/2.0;
      GeoTransform       *b3Transform = new GeoTransform(
							 GeoTrf::Translate3D(xDist[i],0,height)
							 );
      world->add(b3Transform);
      world->add(b3Phys);
    }
  }
  // Add the middle separation bar
  {
    const GeoBox       *b3Box  = new GeoBox(barWidth2/2.0, cutoutDepth/2.0,barThickness/2.0);
    const GeoLogVol    *b3Log  = new GeoLogVol("B3Log", b3Box, Aluminium);
    GeoPhysVol         *b3Phys = new GeoPhysVol(b3Log);
    double xDist=leftRightLegSeparation/2.0-barWidth1/2.0-cutoutWidth-barWidth2/2.0;
    for (int i=0;i<2;i++) {
      double height=platformHeight/2.0+3.0*barThickness/2.0+flangeThickness/2.0;
      GeoTransform       *b3Transform = new GeoTransform(
							 GeoTrf::Translate3D(xDist,0,height)
							 );
      world->add(b3Transform);
      world->add(b3Phys);
    }
  }
  {
    double height=platformHeight/2.0-1.0/8.0/2.0+flangeThickness/2.0+barThickness;
    double xLength=(leftRightLegSeparation+barWidth1)/2.0-barWidth1-cutoutWidth/2.0 ;
    const GeoBox       *b4Box  = new GeoBox(xLength, 27.5/2.0    ,1.0/8.0/2.0);
    const GeoLogVol    *b4Log  = new GeoLogVol("B4Log", b4Box, Aluminium);
    GeoPhysVol         *b4Phys = new GeoPhysVol(b4Log);
    GeoTransform       *b4Transform = new GeoTransform(
							 GeoTrf::Translate3D(-(61.0+11.0/16.0+4.0)/2.0+xLength+4,0,height)
							 );
    world->add(b4Transform);
    world->add(b4Phys);
  } 

  // Add the sink:  a Boolean Volume.
  {
    double sinkDepth=4.5;
    double sinkIntWidth=30.25;
    double sinkIntDepth=16+15./16.;
    const GeoBox       *sBox  = new GeoBox(cutoutWidth/2.0,cutoutDepth/2.0,sinkDepth);
    const GeoBox       *wHole = new GeoBox(sinkIntWidth/2.0, sinkIntDepth/2.0,sinkDepth-0.5);
    const GeoShape &    sink = (sBox->subtract((*wHole)<<GeoTrf::Translate3D(0,-1.5,2.0)));
    const GeoLogVol    *sLog  = new GeoLogVol("SLog", &sink, Stainless);
    GeoPhysVol         *sPhys = new GeoPhysVol(sLog);
    GeoTransform       *sTransform = new GeoTransform(
						      GeoTrf::Translate3D((leftRightLegSeparation+barWidth1)/2.0-barWidth1-cutoutWidth/2.0,0, 15.5)
    );
    world->add(sTransform);
    world->add(sPhys);
  }
  // Add the plaque
  {
    double height=platformHeight/2.0+barThickness/2.0+flangeThickness/2.0+barThickness;
    double xHalfLength=leftRightLegSeparation/2.0-barWidth1/2.0-cutoutWidth/2.0-barWidth2/2.0 ;
    const GeoBox       *wBox  = new GeoBox(xHalfLength, cutoutDepth/2.0    ,barThickness/2.0);
    const GeoLogVol    *wLog  = new GeoLogVol("WLog", wBox, Aluminium);
    GeoPhysVol         *wPhys = new GeoPhysVol(wLog);
    GeoTransform       *wTransform = new GeoTransform(
							 GeoTrf::Translate3D(-(61.0+11.0/16.0+4.0)/2.0+xHalfLength+4,0,height)
							 );
    world->add(wTransform);
    world->add(wPhys);
  }
  
  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" KitchenSinkPlugin *createKitchenSinkPlugin() {
  return new KitchenSinkPlugin;
}

