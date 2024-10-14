/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// Kitchen Sink Plugin
// Joe Boudreau Jan 11 2021
//
// This is an example plugin. It compiles to a shared library 
// (with .so or .dylib extension) which can be viewed with gmex.
//
// This plugin is used to test and develop surfaces, our first 
// example of virtual geometry. 
//
//
// --------------------------------------------------------------------

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoPrintGraphAction.h"
#include "GeoModelKernel/GeoVolumeCursor.h"

#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/GeoTrapezoidSurface.h"
#include "GeoModelKernel/GeoDiamondSurface.h"
#include "GeoModelKernel/GeoVSurface.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelHelpers/defineWorld.h"

#include "GeoModelKernel/GeoSurfaceCursor.h"
#include "GeoModelKernel/GeoCountSurfaceAction.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoAccessSurfaceAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"
using namespace GeoModelKernelUnits;

// Class Declaration

class SurfAlignTestPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  SurfAlignTestPlugin();

  // Destructor:
  ~SurfAlignTestPlugin();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const SurfAlignTestPlugin & operator=(const SurfAlignTestPlugin &right)=delete;
  SurfAlignTestPlugin(const SurfAlignTestPlugin &right) = delete;

};


// Class definition:

// Constructor
SurfAlignTestPlugin::SurfAlignTestPlugin()
{
}

// Destructor
SurfAlignTestPlugin::~SurfAlignTestPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void SurfAlignTestPlugin::create(GeoVPhysVol *world, bool /*publish*/) {
  // Define elements used in this example:
  GeoElement  *oxygen        = new GeoElement("Oxygen",    "O",   19,  39*gram/mole);
  GeoElement  *nitrogen      = new GeoElement("Nitrogen",  "N",   7,  14*gram/mole);
  GeoElement  *argon         = new GeoElement("Argon",     "Ar", 18,  40*gram/mole);
  GeoElement  *aluminium     = new GeoElement("Aluminium", "Al", 13,  26*gram/mole);

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

  GeoBox     *boxShape = new GeoBox(10,15,20);
  GeoLogVol  *boxLog = new GeoLogVol("BoxLog",boxShape,Air);
  GeoPhysVol *boxPhys = new GeoPhysVol(boxLog);


  {

////////////////////////////////////////////////////////////////////////////////////////////////
    GeoBox     *boxShape = new GeoBox(3,7,5);
    GeoLogVol  *boxLog = new GeoLogVol("BoxLog",boxShape,Air);
    GeoFullPhysVol *boxPhys1 = new GeoFullPhysVol(boxLog);
    GeoRectSurface* rectSurface1 = new GeoRectSurface(5, 9);
    GeoVSurface* surf1 = new GeoVSurface(rectSurface1);
    GeoAlignableTransform *move_together1=new GeoAlignableTransform(GeoTrf::TranslateX3D(0));
    //GeoTransform * initial_rot = new GeoTransform(GeoTrf::RotateY3D(1.571));
    
    boxPhys->add(move_together1);
    boxPhys->add(boxPhys1);
    
    boxPhys->add(move_together1);
    //boxPhys->add(initial_rot);
    boxPhys->add(surf1);
    move_together1->setDelta(GeoTrf::RotateZ3D(1)*GeoTrf::RotateY3D(0.5));
    
    GeoBox     *boxShape2 = new GeoBox(4,4,4);
    GeoLogVol  *boxLog2 = new GeoLogVol("BoxLog",boxShape2,Air);
    GeoFullPhysVol *boxPhys4 = new GeoFullPhysVol(boxLog2);
    boxPhys1->add(boxPhys4);
    
    //GeoRectSurface *rectSurface3 = new GeoRectSurface(7, 7);
    //GeoVSurface* surf4 = new GeoVSurface(rectSurface3);
    // GeoTrapezoidSurface* trapezoid = new GeoTrapezoidSurface(9, 7, 5);
    // GeoVSurface* surf4 = new GeoVSurface(trapezoid);
    GeoDiamondSurface* diamond = new GeoDiamondSurface(4, 6, 2, 5, 3);
    GeoVSurface* surf4 = new GeoVSurface(diamond);
    GeoTransform * initial_rot = new GeoTransform(GeoTrf::RotateY3D(1.571));
    boxPhys1->add(initial_rot);
    boxPhys1->add(surf4);
////////////////////////////////////////////////////////////////////////////////////////////////

    GeoFullPhysVol *boxPhys2 = new GeoFullPhysVol(boxLog);
    //GeoRectSurface *rectSurface2 = new GeoRectSurface(8, 8);
    //GeoVSurface* surf2 = new GeoVSurface(rectSurface2);
    // GeoTrapezoidSurface* trapezoid2 = new GeoTrapezoidSurface(4, 6, 8);
    // GeoVSurface* surf2 = new GeoVSurface(trapezoid2);
    GeoDiamondSurface* diamond2 = new GeoDiamondSurface(8, 12, 4, 10, 6);
    GeoVSurface* surf2 = new GeoVSurface(diamond2);
    GeoAlignableTransform *move_together2=new GeoAlignableTransform(GeoTrf::TranslateX3D(-20));
    
    boxPhys->add(move_together2);
    boxPhys->add(boxPhys2);
    boxPhys->add(move_together2);
    boxPhys->add(surf2);
    move_together2->setDelta(GeoTrf::RotateZ3D(0));
    
////////////////////////////////////////////////////////////////////////////////////////////////
    
    GeoFullPhysVol *boxPhys3 = new GeoFullPhysVol(boxLog);
    //GeoRectSurface *rectSurface3 = new GeoRectSurface(8, 8);
    //GeoVSurface* surf3 = new GeoVSurface(rectSurface2);
    GeoTrapezoidSurface* trapezoid2 = new GeoTrapezoidSurface(4, 6, 8);   
    GeoVSurface* surf3 = new GeoVSurface(trapezoid2);
    GeoAlignableTransform *move_together3=new GeoAlignableTransform(GeoTrf::TranslateX3D(20));
    
    boxPhys->add(move_together3);
    boxPhys->add(boxPhys3);
    boxPhys->add(move_together3);
    boxPhys->add(surf3);
    move_together3->setDelta(GeoTrf::RotateX3D(0));

  }
  
    GeoTrapezoidSurface* trapezoid2 = new GeoTrapezoidSurface(9, 12, 8);
    GeoVSurface* surf5 = new GeoVSurface(trapezoid2);
    GeoAlignableTransform *move_together5=new GeoAlignableTransform(GeoTrf::TranslateX3D(40));
    
    boxPhys->add(move_together5);
    boxPhys->add(surf5);
    move_together5->setDelta(GeoTrf::RotateX3D(1));
    
  world->add(boxPhys);
  GeoPrintGraphAction printGraphAction(std::cout);
  world->exec(&printGraphAction);;

  {
    int i = 0;
    
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;    
    std::cout << " cursor at i= " << i << std::endl;
    GeoVolumeCursor cursor(boxPhys);
    //GeoSurfaceCursor cursor(boxPhys);
    
    while (!cursor.atEnd()) {
      i += 1;
      std::cout << "!!! " << std::endl;
      std::cout << cursor.getTransform().rotation() << std::endl;
      std::cout << "!!! " << std::endl; 
      std::cout << cursor.getTransform().translation() << std::endl;
      std::cout << " " << std::endl;
      cursor.next();
    }
  }


  GeoCountSurfaceAction countSurface;
  boxPhys->exec(&countSurface);
  std::cout << "Number of Virtual Surface: " << countSurface.getCount() << std::endl;
  unsigned int totalVSurface = countSurface.getCount();
  
  for (unsigned int i = 0; i< totalVSurface; i++){
    GeoAccessSurfaceAction surfaceAccess (i, nullptr);
    boxPhys->exec(&surfaceAccess);
    std::cout << "surface transform rotation: " << surfaceAccess.getTransform().rotation() << std::endl;
    std::cout << "surface transform trans: " << surfaceAccess.getTransform().translation() << std::endl;
  }
   std::cout << "surface5 transform trans: " << surf5->getX().rotation() << std::endl;
   std::cout << "surface5 transform trans: " << surf5->getX().translation() << std::endl;
  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SurfAlignTestPlugin *createSurfAlignTestPlugin() {
  return new SurfAlignTestPlugin;
}

