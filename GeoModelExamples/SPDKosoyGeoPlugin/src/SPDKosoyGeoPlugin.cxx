/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// SPD Alternative sextant straw tracker detector ("Kosoy" prototype)
// Ramdas Makhmanazarov May 29 2025
//
// This is an example plugin of SPD sextant straw tracker detector.
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

#include <cmath>

// Class Declaration

class SPDKosoyGeoPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  SPDKosoyGeoPlugin();

  // Destructor:
  ~SPDKosoyGeoPlugin();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish=false);

  // Helper filler

  void fillTrapezoid(GeoVPhysVol *world, GeoPhysVol *oPhys, double cShift, double oRad, int nTubes, double downShift);

 private:

  // Illegal operations:
  const SPDKosoyGeoPlugin & operator=(const SPDKosoyGeoPlugin &right)=delete;
  SPDKosoyGeoPlugin(const SPDKosoyGeoPlugin &right) = delete;

};


// Class definition:

// Constructor
SPDKosoyGeoPlugin::SPDKosoyGeoPlugin()
{
}

// Destructor
SPDKosoyGeoPlugin::~SPDKosoyGeoPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void SPDKosoyGeoPlugin::create(GeoVPhysVol *world, bool /*publish*/) {
  const double degree = M_PI / 180.0;

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

  double t1TubeLength = 1.7*m;                            // Overall length of tube t1;
  double iRad=0.9/2.0*cm;                          // 3/4 inch (inner diameter) pipe
  double oRad=1.0/2.0*cm;                          // 1-1/16   (outer diameter) 
  double cShift    = sqrt(3.)*cm/2;                 // for circles compact placing
  double alpha = 2.5*1; // red and blue OY rotation angle
  // For imagination
  {
    // const GeoTube      *t1axes    = new GeoTube(0,outerRadius, t1TubeLength/2.0);
    // const GeoLogVol    *t1Log     = new  GeoLogVol("T1Logax", t1axes, Aluminium);
    // GeoPhysVol         *t1axPhys    = new GeoPhysVol(t1Log);
    // GeoTransform       *xax       = new GeoTransform(GeoTrf::RotateX3D(45.0*degree)*GeoTrf::Translate3D(0, 0, 0));
    
    // //GeoTransform       *t2Transform = new GeoTransform(GeoTrf::RotateX3D(90.0*degree)*GeoTrf::TranslateX3D((1-2.0*i)*leftRightLegSeparation/2.0));

    // world->add(xax);
    // world->add(t1axPhys);
  }
  // Add the four legs of the kitchen sink:
  {
    const GeoTube      *t1Tube    = new GeoTube(iRad,oRad, t1TubeLength/2.0);
    const GeoLogVol    *t1Log     = new  GeoLogVol("T1Log", t1Tube, Aluminium);
    GeoPhysVol         *t1Phys    = new GeoPhysVol(t1Log);

    int nTubes  = 34; // from first layer
    

    // function only if nTubes/2 of first layer is integer  
    fillTrapezoid(world, t1Phys, cShift, oRad, nTubes, 0);

    // red and blue tubes
    

    const GeoTube      *tRedTube    = new GeoTube(iRad,oRad, (t1TubeLength/cos(alpha*degree))/2);
    const GeoLogVol    *tRedLog     = new  GeoLogVol("TRedLog", tRedTube, Aluminium);
    GeoPhysVol         *tRedPhys    = new GeoPhysVol(tRedLog);

    const GeoTube      *tBlueTube    = new GeoTube(iRad,oRad, (t1TubeLength/cos(alpha*degree))/2);
    const GeoLogVol    *tBlueLog     = new  GeoLogVol("TBlueLog", tBlueTube, Aluminium);
    GeoPhysVol         *tBluePhys    = new GeoPhysVol(tBlueLog);

    double downShift1 = 7*cShift+2*oRad;
    int nTubesR = 47; // first red
    
    
    for (int i=0; i<23; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift1+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift1+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }
    for (int i=0; i<23; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==22){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }

    for (int i=0; i<22; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift1+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift1+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==21){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift1+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift1+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<22; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift1+2*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift1+2*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    for (int i=0; i<21; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift1+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift1+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==20){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift1+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift1+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }
    for (int i=0; i<21; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift1+3*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift1+3*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }

    for (int i=0; i<20; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift1+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift1+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==19){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift1+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift1+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<20; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift1+4*cShift+6*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift1+4*cShift+6*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }


    // Second gr Tube-Trapezoid (C1)
    const GeoLogVol    *t2Log     = new  GeoLogVol("T2Log", t1Tube, Aluminium);
    GeoPhysVol         *t2Phys    = new GeoPhysVol(t2Log);

    nTubes  =58; //from last

    //skip red and blues tubes
    double downShift2 = downShift1 + (2*oRad+cShift+2*oRad+cShift)*2;

    for (int i=0; i<29; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(oRad+(i+1)*cm, -(downShift2+7*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(oRad-i*cm, -(downShift2+7*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<28; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift2+6*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift2+6*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==27){
        GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift2+6*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    for (int i=0; i<28; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(oRad+(i+1)*cm, -(downShift2+5*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(oRad-i*cm, -(downShift2+5*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<27; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift2+4*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift2+4*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==26){
        GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift2+4*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    for (int i=0; i<27; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(oRad+(i+1)*cm, -(downShift2+3*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(oRad-i*cm, -(downShift2+3*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<26; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift2+2*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift2+2*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==25){
        GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift2+2*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    for (int i=0; i<26; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(oRad+(i+1)*cm, -(downShift2+1*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(oRad-i*cm, -(downShift2+1*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<25; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift2+0*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift2+0*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==24){
        GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift2+0*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }
    
    // Second red and blue layer (same as previous)

    double downShift3 =downShift2 + 7*cShift+2*oRad;
    
    for (int i=0; i<23; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift3+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift3+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }
    for (int i=0; i<23; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==22){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }

    for (int i=0; i<22; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift3+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift3+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==21){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift3+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift3+1*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<22; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift3+2*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift3+2*cShift+2*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    for (int i=0; i<21; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift3+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift3+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==20){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift3+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift3+2*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }
    for (int i=0; i<21; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift3+3*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift3+3*cShift+4*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }

    for (int i=0; i<20; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*oRad+(i+1)*cm, -(downShift3+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*oRad-(i)*cm, -(downShift3+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==19){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(oRad-(i-2)*cm, -(downShift3+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(oRad+(i+2)*cm, -(downShift3+3*cShift+6*oRad), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<20; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*oRad+(i+2)*cm, -(downShift3+4*cShift+6*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*oRad-(i-1)*cm, -(downShift3+4*cShift+6*oRad), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    // Third green tubes trapezoid
    double downShift4 = downShift3 + (2*oRad+cShift+2*oRad+cShift)*2;
    nTubes = 68;
    fillTrapezoid(world, t1Phys, cShift, oRad, nTubes, -downShift4);



  }
  

  //--------------------------------------//
}

//// function only if nTubes/2 of first layer is integer!
void SPDKosoyGeoPlugin::fillTrapezoid(GeoVPhysVol *world, GeoPhysVol *oPhys, double cShift, double oRad, int nTubes, double downShift){
  
  int nLayers = 8;

  for (int j=0; j<nLayers/2; j++){
    for (int i=0; i<(nTubes/2)+j; i++){
      GeoTransform  *xform1    = new GeoTransform(GeoTrf::Translate3D(oRad+(i+1)*cm, -2*j*cShift+downShift, 0));
      world->add(xform1);
      world->add(oPhys);
      GeoTransform  *xform2    = new GeoTransform(GeoTrf::Translate3D(oRad-i*cm, -2*j*cShift+downShift, 0));
      world->add(xform2);
      world->add(oPhys);
      GeoTransform  *xform3    = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(2*j+1)*cShift+downShift, 0));
      world->add(xform3);
      world->add(oPhys);
      GeoTransform  *xform4    = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(2*j+1)*cShift+downShift, 0));
      world->add(xform4);
      world->add(oPhys);
      if (i==(nTubes/2)+j-1){
        GeoTransform  *xform4  = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(2*j+1)*cShift+downShift, 0));
        world->add(xform4);
        world->add(oPhys);
      }
    }
  }


}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SPDKosoyGeoPlugin *createSPDKosoyGeoPlugin() {
  return new SPDKosoyGeoPlugin;
}



