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


  {
/*
// define a virtual surface
  GeoRectSurface *rectSurface = new GeoRectSurface(5, 5);
  GeoAlignableTransform *surf_rot=new GeoAlignableTransform(GeoTrf::TranslateY3D(18));  
  boxPhys->add(surf_rot);
  boxPhys->add(rectSurface);
  surf_rot->setDelta(GeoTrf::RotateX3D(1.2));
  */
//
    GeoBox     *boxShape = new GeoBox(5,5,5);
    GeoLogVol  *boxLog = new GeoLogVol("BoxLog",boxShape,Air);
    GeoPhysVol *boxPhys2 = new GeoPhysVol(boxLog);

    GeoAlignableTransform *xf_initial=new GeoAlignableTransform(GeoTrf::TranslateZ3D(3));
    boxPhys->add(xf_initial);
    boxPhys->add(boxPhys2);
    xf_initial->setDelta(GeoTrf::RotateX3D(1.2));

// define a virtual surface
  GeoRectSurface* rectSurface = new GeoRectSurface(5, 5);
/*  GeoVSurface* surf = new GeoVSurface(rectSurface);
  GeoAlignableTransform *surf_rot=new GeoAlignableTransform(GeoTrf::TranslateY3D(10));  
  boxPhys->add(surf_rot);
  boxPhys->add(surf);
  surf_rot->setDelta(GeoTrf::RotateY3D(1.2));*/


// define a virtual surface2
  GeoRectSurface *rectSurface2 = new GeoRectSurface(5, 7.5);
/*  GeoVSurface* surf2 = new GeoVSurface(rectSurface2);
  GeoAlignableTransform *surf_rot2=new GeoAlignableTransform(GeoTrf::TranslateX3D(-20));  
  boxPhys->add(surf_rot2);
  boxPhys->add(surf2);
  surf_rot2->setDelta(GeoTrf::RotateY3D(0.8));*/
//

    GeoAlignableTransform *xf=new GeoAlignableTransform(GeoTrf::TranslateZ3D(18));
    GeoPhysVol *boxPhys3 = new GeoPhysVol(boxLog);
    GeoPhysVol *boxPhys4 = new GeoPhysVol(boxLog);
    
    boxPhys3->add(xf);
    boxPhys3->add(boxPhys4);
    
  GeoRectSurface *rectSurface2_2 = new GeoRectSurface(7.5, 7.5);
/*  GeoVSurface* surf2_2 = new GeoVSurface(rectSurface2_2);
  GeoAlignableTransform *surf_rot2_2 = new GeoAlignableTransform(GeoTrf::TranslateX3D(-10));  
  boxPhys3->add(surf_rot2_2);
  boxPhys3->add(surf2_2);
  surf_rot2_2->setDelta(GeoTrf::RotateY3D(1.0));*/    
   
   boxPhys->add(xf);
    boxPhys->add(boxPhys3);
    xf->setDelta(GeoTrf::RotateX3D(0.1)*GeoTrf::RotateY3D(0.15));

// define a virtual surface3
  GeoRectSurface *rectSurface3 = new GeoRectSurface(10, 12);
/*  GeoVSurface* surf3 = new GeoVSurface(rectSurface3);
  GeoAlignableTransform *surf_rot3=new GeoAlignableTransform(GeoTrf::TranslateX3D(10)*GeoTrf::TranslateY3D(15)*GeoTrf::TranslateZ3D(20));  
  boxPhys->add(surf_rot3);
  boxPhys->add(surf3);
  surf_rot3->setDelta(GeoTrf::RotateZ3D(0.9));*/
//    

  }
  GeoLogVol  *boxLog5 = new GeoLogVol("BoxLog5",boxShape,Air);
  GeoPhysVol *boxPhys5 = new GeoPhysVol(boxLog5);
  world->add(boxPhys);
  //world->add(boxPhys5);
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

/*  
  GeoCountVolAction countVol;
  boxPhys->exec(&countVol);
  std::cout << "Number of Physical Volume: " << countVol.getCount() << std::endl;  

  unsigned int index = 2;
  GeoAccessVolumeAction volumeAccess (index, nullptr);
  boxPhys->exec(&volumeAccess);
  std::cout << "volume transform rotation: " << volumeAccess.getTransform().rotation() << std::endl;
  std::cout << "volume transform trans: " << volumeAccess.getTransform().translation() << std::endl;
*/ 

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
 
  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SurfaceTestPlugin *createSurfaceTestPlugin() {
  return new SurfaceTestPlugin;
}

