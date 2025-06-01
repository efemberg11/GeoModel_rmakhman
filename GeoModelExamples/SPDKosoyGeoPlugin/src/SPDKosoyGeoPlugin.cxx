/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// Sextant-based geometry of the SPD straw tracker ("Kosoy" prototype)
// Ramdas Makhmanazarov May 29 2025
//
// This is an example plugin of SPD sextant straw tracker detector.
// Geometry information taken from:
// https://indico.jinr.ru/event/5231/contributions/30361/attachments/21672/38135/Ruslan_SPD&MC_Meeting_2025-02-19.pdf
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
 
  // Build Sextant A
  void buildSextantA(GeoVPhysVol *world, double cShift, double outerRadius,
    double innerRadius, double alpha, double tubeLength, GeoMaterial *tubeMaterial);
  // Build Sextant B
  void buildSextantB(GeoVPhysVol *world, double cShift, double outerRadius,
    double innerRadius, double alpha, double tubeLength, GeoMaterial *tubeMaterial);

  // Helper filler
  void fillTrapezoid(GeoVPhysVol *world, GeoPhysVol *oPhys, double cShift, double outerRadius,
    int nTubes, double downShift);

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

void SPDKosoyGeoPlugin::create(GeoVPhysVol *world, bool /*publish*/) {
  const double degree = M_PI / 180.0;

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

  

  // Some dimensions used below:

  double t1TubeLength = 1.7*m;                            // Overall length of tube t1;
  double iRad=0.9/2.0*cm;                          // inner raduis
  double outerRadius=1.0/2.0*cm;                          // outer radius
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
  // Build trapezoidal volumes from tubes
  
  buildSextantA(world, cShift, outerRadius, iRad, alpha, t1TubeLength, Aluminium);
  buildSextantB(world, cShift, outerRadius, iRad, alpha, t1TubeLength, Aluminium);
  
  


  //--------------------------------------//
}

//// function only if nTubes/2 of first layer is integer!
void SPDKosoyGeoPlugin::fillTrapezoid(GeoVPhysVol *world, GeoPhysVol *oPhys,
   double cShift, double outerRadius, int nTubes, double downShift){
  
  int nLayers = 8;

  for (int j=0; j<nLayers/2; j++){
    for (int i=0; i<(nTubes/2)+j; i++){
      GeoTransform  *xform1    = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -2*j*cShift+downShift, 0));
      world->add(xform1);
      world->add(oPhys);
      GeoTransform  *xform2    = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -2*j*cShift+downShift, 0));
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

void SPDKosoyGeoPlugin::buildSextantB(GeoVPhysVol *world, double cShift, double outerRadius,
  double innerRadius, double alpha, double tubeLength, GeoMaterial *tubeMaterial){

  const GeoTube      *t1Tube    = new GeoTube(innerRadius,outerRadius, tubeLength/2.0);
  const GeoLogVol    *t1Log     = new  GeoLogVol("T1Log", t1Tube, tubeMaterial);
  GeoPhysVol         *t1Phys    = new GeoPhysVol(t1Log);
  
  int nTubes = 32;
  double downShift0 = 1*sqrt(3)*33*cm/2.;
      // function only if nTubes/2 of first layer is integer  

  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, downShift0 );

  // Red and blue tubes
    
  const GeoTube      *tRedTube    = new GeoTube(innerRadius,outerRadius, (tubeLength/cos(alpha*degree))/2);
  const GeoLogVol    *tRedLog     = new  GeoLogVol("TRedLog", tRedTube, tubeMaterial);
  GeoPhysVol         *tRedPhys    = new GeoPhysVol(tRedLog);

  const GeoTube      *tBlueTube    = new GeoTube(innerRadius,outerRadius, (tubeLength/cos(alpha*degree))/2);
  const GeoLogVol    *tBlueLog     = new  GeoLogVol("TBlueLog", tBlueTube, tubeMaterial);
  GeoPhysVol         *tBluePhys    = new GeoPhysVol(tBlueLog);

  double downShift1 = downShift0+7*cShift+2*outerRadius;

  for (int i=0; i<15; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift1+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift1+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
  }
  for (int i=0; i<15; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
    if (i==14){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
    }
  }

  for (int i=0; i<14; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
    if (i==13){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
    }
  }
  for (int i=0; i<14; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift1+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift1+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
  }

  for (int i=0; i<13; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
    if (i==12){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
    }
  }
  for (int i=0; i<13; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift1+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift1+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
  }

  for (int i=0; i<12; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
    if (i==11){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
    }
  }
  for (int i=0; i<12; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift1+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift1+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
  }

  // Second green Tube-Trapezoid
  // const GeoLogVol    *t2Log     = new  GeoLogVol("T2Log", t1Tube, tubeMaterial);
  // GeoPhysVol         *t2Phys    = new GeoPhysVol(t2Log);

  nTubes  =56; //from last

  //skip red and blues tubes
  double downShift2 = downShift1 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;

  for (int i=0; i<28; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift2+7*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift2+7*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<27; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift2+6*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift2+6*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==26){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift2+6*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }

  for (int i=0; i<27; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift2+5*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift2+5*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<26; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift2+4*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift2+4*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==25){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift2+4*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }

  for (int i=0; i<26; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift2+3*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift2+3*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<25; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift2+2*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift2+2*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==24){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift2+2*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }

  for (int i=0; i<25; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift2+1*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift2+1*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<24; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift2+0*cShift), 0));
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift2+0*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==23){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift2+0*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }
  
  // Second red and blue layer (same as previous)
  double downShift3 = downShift2+7*cShift+2*outerRadius;
  
  for (int i=0; i<15; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift3+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift3+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
  }
  for (int i=0; i<15; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
    if (i==14){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
    }
  }

  for (int i=0; i<14; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
    if (i==13){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
    }
  }
  for (int i=0; i<14; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift3+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift3+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
  }

  for (int i=0; i<13; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
    if (i==12){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
    }
  }
  for (int i=0; i<13; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift3+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift3+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
  }

  for (int i=0; i<12; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
    if (i==11){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
    }
  }
  for (int i=0; i<12; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift3+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift3+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
  }

  // third green tubes layer
  double downShift4 = downShift3 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
  nTubes = 66;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, downShift4);

  // third red and blue tubes layer
  double downShift5 = downShift4 + 7*cShift+2*outerRadius;
  // nTubes = 63 from first (56 from last)

  for (int i=0; i<31; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift5+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift5+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
  }
  for (int i=0; i<31; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
    if (i==30){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
    }
  }

  for (int i=0; i<30; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
    if (i==29){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
    }
  }
  for (int i=0; i<30; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift5+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift5+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
  }

  for (int i=0; i<29; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
    if (i==28){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
    }
  }
  for (int i=0; i<29; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift5+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform1);
    world->add(tRedPhys);      
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift5+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
    world->add(xform2);
    world->add(tRedPhys);
  }

  for (int i=0; i<28; i++){
    GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, (downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, (downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
    if (i==27){
      GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, (downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, (downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
    }
  }
  for (int i=0; i<28; i++){
    GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, (downShift5+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform1);
    world->add(tBluePhys);
    GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, (downShift5+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
    world->add(xform2);
    world->add(tBluePhys);
  }

  double downShift6 = downShift5 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;

  // Last green trapezoid (6 layers, 88 tubes from last)

  for (int i=0; i<44; i++){
    GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+5*cShift), 0));
    GeoTransform  *xform2   =     new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift6+5*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<43; i++){
    GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+4*cShift), 0));
    GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+4*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==42){
      GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+4*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }

  for (int i=0; i<43; i++){
    GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+3*cShift), 0));
    GeoTransform  *xform2   =     new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift6+3*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<42; i++){
    GeoTransform  *xform1    = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+2*cShift), 0));
    GeoTransform  *xform2    =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+2*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==41){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+2*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }

  for (int i=0; i<42; i++){
    GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+1*cShift), 0));
    GeoTransform  *xform2   =     new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, (downShift6+1*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
  }
  for (int i=0; i<41; i++){
    GeoTransform  *xform1    = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+0*cShift), 0));
    GeoTransform  *xform2    =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+0*cShift), 0));
    world->add(xform1);
    world->add(t1Phys);
    world->add(xform2);
    world->add(t1Phys);
    if (i==40){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+0*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
    }
  }
  
  // Arc bordered tube filling
  for (int i=0; i<43; i++){

    if (i<43){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+6*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+6*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==42){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+6*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }
    
    if (i<42){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+7*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, (downShift6+7*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
    }

    if (i<40){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+8*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+8*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==39){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+8*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    if (i<38){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+9*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, (downShift6+9*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
    }

    if (i<36){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+10*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+10*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==35){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+10*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    if (i<34){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+11*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, (downShift6+11*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
    }

    if (i<32){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+12*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+12*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==31){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+12*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    if (i<30){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+13*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, (downShift6+13*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
    }

    if (i<27){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+14*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+14*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==26){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+14*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    if (i<25){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+15*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, (downShift6+15*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
    }

    if (i<21){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+16*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+16*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==20){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+16*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    if (i<17){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, (downShift6+17*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, (downShift6+17*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
    }

    if (i<12){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, (downShift6+18*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, (downShift6+18*cShift), 0));
      world->add(xform2);
      world->add(t1Phys);
      if (i==11){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, (downShift6+18*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

  }

  }

void SPDKosoyGeoPlugin::buildSextantA(GeoVPhysVol *world, double cShift, double outerRadius,
   double innerRadius, double alpha, double tubeLength, GeoMaterial *tubeMaterial){
    //Green tubes
    const GeoTube      *t1Tube    = new GeoTube(innerRadius,outerRadius, tubeLength/2.0);
    const GeoLogVol    *t1Log     = new  GeoLogVol("T1Log", t1Tube, tubeMaterial);
    GeoPhysVol         *t1Phys    = new GeoPhysVol(t1Log);

    int nTubes  = 34; // from first layer
    double downShift0 = 1*sqrt(3)*33*cm/2.;
    // function only if nTubes/2 of first layer is integer  
    fillTrapezoid(world, t1Phys, cShift, outerRadius, nTubes, -downShift0);

    // Red and blue tubes
  
    const GeoTube      *tRedTube    = new GeoTube(innerRadius,outerRadius, (tubeLength/cos(alpha*degree))/2);
    const GeoLogVol    *tRedLog     = new  GeoLogVol("TRedLog", tRedTube, tubeMaterial);
    GeoPhysVol         *tRedPhys    = new GeoPhysVol(tRedLog);

    const GeoTube      *tBlueTube    = new GeoTube(innerRadius,outerRadius, (tubeLength/cos(alpha*degree))/2);
    const GeoLogVol    *tBlueLog     = new  GeoLogVol("TBlueLog", tBlueTube, tubeMaterial);
    GeoPhysVol         *tBluePhys    = new GeoPhysVol(tBlueLog);

    double downShift1 = downShift0+7*cShift+2*outerRadius;
    int nTubesR = 47; // first red
    
    
    for (int i=0; i<23; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift1+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift1+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }
    for (int i=0; i<23; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==22){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift1+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }

    for (int i=0; i<22; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==21){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift1+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<22; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift1+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift1+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    for (int i=0; i<21; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==20){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift1+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }
    for (int i=0; i<21; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift1+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift1+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }

    for (int i=0; i<20; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==19){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift1+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<20; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift1+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift1+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }


    // Second gr Tube-Trapezoid (C1)
    // const GeoLogVol    *t2Log     = new  GeoLogVol("T2Log", t1Tube, tubeMaterial);
    // GeoPhysVol         *t2Phys    = new GeoPhysVol(t2Log);

    nTubes  =58; //from last

    //skip red and blues tubes
    double downShift2 = downShift1 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;

    for (int i=0; i<29; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift2+7*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift2+7*cShift), 0));
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
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift2+5*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift2+5*cShift), 0));
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
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift2+3*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift2+3*cShift), 0));
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
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift2+1*cShift), 0));
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift2+1*cShift), 0));
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

    double downShift3 =downShift2 + 7*cShift+2*outerRadius;
    
    for (int i=0; i<23; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift3+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift3+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }
    for (int i=0; i<23; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==22){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift3+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }

    for (int i=0; i<22; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==21){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift3+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<22; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift3+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift3+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    for (int i=0; i<21; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==20){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift3+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }
    for (int i=0; i<21; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift3+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift3+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }

    for (int i=0; i<20; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==19){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift3+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<20; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift3+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift3+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    // Third green tubes trapezoid
    double downShift4 = downShift3 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
    nTubes = 68;
    fillTrapezoid(world, t1Phys, cShift, outerRadius, nTubes, -downShift4);

    // Third red and blue trapezoid

    double downShift5 = downShift4 + 7*cShift + 2*outerRadius;

    for (int i=0; i<40; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift5+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift5+1*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }
    for (int i=0; i<40; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==39){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift5+0*cShift), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }

    for (int i=0; i<39; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==38){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift5+1*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<39; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift5+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift5+2*cShift+2*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    for (int i=0; i<38; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
      if (i==37){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform2);
        world->add(tRedPhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift5+2*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
        world->add(xform1);
        world->add(tRedPhys);
      }
    }
    for (int i=0; i<38; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift5+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform1);
      world->add(tRedPhys);      
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift5+3*cShift+4*outerRadius), 0)*GeoTrf::RotateY3D(-alpha*degree));
      world->add(xform2);
      world->add(tRedPhys);
    }

    for (int i=0; i<37; i++){
      GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius+(i+1)*cm, -(downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2  = new GeoTransform(GeoTrf::Translate3D(1*outerRadius-(i)*cm, -(downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
      if (i==36){
        GeoTransform  *xform2= new GeoTransform(GeoTrf::Translate3D(outerRadius-(i-2)*cm, -(downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform2);
        world->add(tBluePhys);
        GeoTransform  *xform1= new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+2)*cm, -(downShift5+3*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
        world->add(xform1);
        world->add(tBluePhys);
      }
    }
    for (int i=0; i<37; i++){
      GeoTransform  *xform1         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius+(i+2)*cm, -(downShift5+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform1);
      world->add(tBluePhys);
      GeoTransform  *xform2         = new GeoTransform(GeoTrf::Translate3D(0*outerRadius-(i-1)*cm, -(downShift5+4*cShift+6*outerRadius), 0)*GeoTrf::RotateY3D(alpha*degree));
      world->add(xform2);
      world->add(tBluePhys);
    }

    // Fouth gren tubes trapezoid (here 6 layers)
    double downShift6 = downShift5 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;

    nTubes = 90; // from last
    for (int i=0; i<45; i++){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+5*cShift), 0));
      GeoTransform  *xform2   =     new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift6+5*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<44; i++){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+4*cShift), 0));
      GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+4*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==43){
        GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+4*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    for (int i=0; i<44; i++){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+3*cShift), 0));
      GeoTransform  *xform2   =     new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift6+3*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<43; i++){
      GeoTransform  *xform1    = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+2*cShift), 0));
      GeoTransform  *xform2    =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+2*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==42){
        GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+2*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }

    for (int i=0; i<43; i++){
      GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+1*cShift), 0));
      GeoTransform  *xform2   =     new GeoTransform(GeoTrf::Translate3D(outerRadius-i*cm, -(downShift6+1*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
    }
    for (int i=0; i<42; i++){
      GeoTransform  *xform1    = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+0*cShift), 0));
      GeoTransform  *xform2    =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+0*cShift), 0));
      world->add(xform1);
      world->add(t1Phys);
      world->add(xform2);
      world->add(t1Phys);
      if (i==41){
        GeoTransform  *xform1  = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+0*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
      }
    }
    
    // Arc bordered tube filling
    for (int i=0; i<43; i++){

      if (i<43){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+6*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+6*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==42){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+6*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }
      
      if (i<42){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+7*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, -(downShift6+7*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
      }

      if (i<40){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+8*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+8*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==39){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+8*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }

      if (i<38){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+9*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, -(downShift6+9*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
      }

      if (i<36){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+10*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+10*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==35){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+10*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }

      if (i<34){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+11*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, -(downShift6+11*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
      }

      if (i<32){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+12*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+12*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==31){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+12*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }

      if (i<30){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+13*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, -(downShift6+13*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
      }

      if (i<27){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+14*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+14*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==26){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+14*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }

      if (i<25){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+15*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, -(downShift6+15*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
      }

      if (i<21){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+16*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+16*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==20){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+16*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }

      if (i<17){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D(outerRadius+(i+1)*cm, -(downShift6+17*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =   new GeoTransform(GeoTrf::Translate3D(outerRadius-(i)*cm, -(downShift6+17*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
      }

      if (i<12){
        GeoTransform  *xform1   = new GeoTransform(GeoTrf::Translate3D((i+1)*cm, -(downShift6+18*cShift), 0));
        world->add(xform1);
        world->add(t1Phys);
        GeoTransform  *xform2   =  new GeoTransform(GeoTrf::Translate3D(-(i)*cm, -(downShift6+18*cShift), 0));
        world->add(xform2);
        world->add(t1Phys);
        if (i==11){
          GeoTransform  *xform1 = new GeoTransform(GeoTrf::Translate3D((i+2)*cm, -(downShift6+18*cShift), 0));
          world->add(xform1);
          world->add(t1Phys);
        }
      }
  
    }

  }

 


// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SPDKosoyGeoPlugin *createSPDKosoyGeoPlugin() {
  return new SPDKosoyGeoPlugin;
}



