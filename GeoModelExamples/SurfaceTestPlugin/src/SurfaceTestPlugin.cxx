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
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelHelpers/defineWorld.h"

#include "GeoModelKernel/GeoSurfaceCursor.h"
using namespace GeoModelKernelUnits;

// Class Declaration

class SurfaceTestPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  SurfaceTestPlugin();

  // Destructor:
  ~SurfaceTestPlugin();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const SurfaceTestPlugin & operator=(const SurfaceTestPlugin &right)=delete;
  SurfaceTestPlugin(const SurfaceTestPlugin &right) = delete;

};


// Class definition:

// Constructor
SurfaceTestPlugin::SurfaceTestPlugin()
{
}

// Destructor
SurfaceTestPlugin::~SurfaceTestPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void SurfaceTestPlugin::create(GeoVPhysVol *world, bool /*publish*/) {
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

// define a virtual surface
  GeoRectSurface *rectSurface = new GeoRectSurface(5, 7.5);
  GeoAlignableTransform *surf_rot=new GeoAlignableTransform(GeoTrf::TranslateZ3D(12));  
  boxPhys->add(surf_rot);
  boxPhys->add(rectSurface);
  surf_rot->setDelta(GeoTrf::RotateX3D(0.1)*GeoTrf::RotateY3D(0.15));
//
  {
    GeoBox     *boxShape = new GeoBox(5,5,5);
    GeoLogVol  *boxLog = new GeoLogVol("BoxLog",boxShape,Air);
    GeoPhysVol *boxPhys2 = new GeoPhysVol(boxLog);
    boxPhys->add(boxPhys2);

    GeoAlignableTransform *xf=new GeoAlignableTransform(GeoTrf::TranslateZ3D(12));
    GeoPhysVol *boxPhys3 = new GeoPhysVol(boxLog);

    boxPhys->add(xf);
    boxPhys->add(boxPhys3);

    xf->setDelta(GeoTrf::RotateX3D(0.1)*GeoTrf::RotateY3D(0.15));
  }
  
  world->add(boxPhys);

  GeoPrintGraphAction printGraphAction(std::cout);
  world->exec(&printGraphAction);;

  {
    int i = 0;
    
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;    
    std::cout << " cursor at i= " << i << std::endl;
    //GeoVolumeCursor cursor(boxPhys);
    GeoSurfaceCursor cursor(boxPhys);
    
    while (!cursor.atEnd()) {
      i += 1;
      //std::cout << "!!! " << cursor.getVolume()->getLogVol() << std::endl;
      std::cout << "!!! " << cursor.getTransform().rotation() << std::endl;
      std::cout << "!!! " << cursor.getTransform().translation() << std::endl;
      std::cout << " " << std::endl;
      std::cout << " " << std::endl;
      std::cout << " cursor at i= " << i << std::endl;
      cursor.next();
      //std::cout << " FINISHED NEXT " << std::endl;
      std::cout << " " << std::endl;
    }
  }

  
  
  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SurfaceTestPlugin *createSurfaceTestPlugin() {
  return new SurfaceTestPlugin;
}

