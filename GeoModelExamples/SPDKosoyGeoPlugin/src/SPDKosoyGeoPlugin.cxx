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
  void buildSextantA(GeoVPhysVol *world, GeoPhysVol *t1Phys,GeoPhysVol *tRedPhys, 
    GeoPhysVol *tBluePhys, double cShift, double outerRadius, double alphaY, double angZ);
  // Build Sextant B
  void buildSextantB(GeoVPhysVol *world, GeoPhysVol *t1Phys,GeoPhysVol *tRedPhys, 
    GeoPhysVol *tBluePhys, double cShift, double outerRadius, double alphaY, double angZ);

  // Helper filler
  void fillLayer(GeoVPhysVol *world, GeoPhysVol *oPhys, double cShift, double outerRadius,
    int nTubes, int nLayers, double downShift, double angZ);
  
  // for green tubes
  void fillTrapezoid(GeoVPhysVol *world, GeoPhysVol *oPhys, double cShift, double outerRadius,
    int nTubes, int nLayers, double downShift, double angZ);
  
    // for red and blue tubes
  void fillTrapezoid2(GeoVPhysVol *world, GeoPhysVol *redPhys, GeoPhysVol *bluePhys,
    double cShift, double outerRadius, int nTubes, double downShift, double angleY, double angZ);

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

  double t1TubeLength = 1.7*m;          // Overall length of tube t1;
  double iRad=0.9/2.0*cm;               // inner raduis
  double outerRadius=1.0/2.0*cm;        // outer radius
  double cShift    = sqrt(3.)*cm/2;     // for circles compact placing
  double alpha = 3*1;                   // red and blue OY rotation angle
  double angZ = 0;                      // sector rotation angle
  
  // Create tube volumes

  // Green
  const GeoTube      *t1Tube    = new GeoTube(iRad,outerRadius, t1TubeLength/2.0);
  const GeoLogVol    *t1Log     = new  GeoLogVol("T1Log", t1Tube, Aluminium);
  GeoPhysVol         *t1Phys    = new GeoPhysVol(t1Log);

  // Red and blue tubes 
  const GeoTube      *tRedTube    = new GeoTube(iRad,outerRadius, (t1TubeLength/cos(alpha*degree))/2);
  const GeoLogVol    *tRedLog     = new  GeoLogVol("TRedLog", tRedTube, Aluminium);
  GeoPhysVol         *tRedPhys    = new GeoPhysVol(tRedLog);

  const GeoTube      *tBlueTube    = new GeoTube(iRad,outerRadius, (t1TubeLength/cos(alpha*degree))/2);
  const GeoLogVol    *tBlueLog     = new  GeoLogVol("TBlueLog", tBlueTube, Aluminium);
  GeoPhysVol         *tBluePhys    = new GeoPhysVol(tBlueLog);

  
  // Build Sectors from trapezoids add fill barrel (currently world) 
  buildSextantA(world, t1Phys, tRedPhys, tBluePhys, cShift, outerRadius, alpha, 300);
  buildSextantA(world, t1Phys, tRedPhys, tBluePhys, cShift, outerRadius, alpha, 180);
  buildSextantA(world, t1Phys, tRedPhys, tBluePhys, cShift, outerRadius, alpha, 60);
  buildSextantB(world, t1Phys, tRedPhys, tBluePhys, cShift, outerRadius, alpha, 0);
  buildSextantB(world, t1Phys, tRedPhys, tBluePhys, cShift, outerRadius, alpha, 120);
  buildSextantB(world, t1Phys, tRedPhys, tBluePhys, cShift, outerRadius, alpha, 240);

  //--------------------------------------//
}

// trapezoid from green tubes
void SPDKosoyGeoPlugin::fillTrapezoid(GeoVPhysVol *world, GeoPhysVol *oPhys,
  double cShift, double outerRadius, int nTubes, int nLayers, double downShift, double angZ){
 
  bool isEven = (nTubes % 2 == 0) ? true : false;

  if (isEven){
    for (int j=0; j<(nLayers/2); j++){
      for (int i=0; i<(nTubes/2 +1)+j; i++){
        GeoTransform  *xform1    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(outerRadius+(i-1)*cm, -2*j*cShift+downShift, 0));
        world->add(xform1);
        world->add(oPhys);
        GeoTransform  *xform2    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(outerRadius-i*cm, -2*j*cShift+downShift, 0));
        world->add(xform2);
        world->add(oPhys);
        GeoTransform  *xform3    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(-(i)*cm, -(2*j+1)*cShift+downShift, 0));
        world->add(xform3);
        world->add(oPhys);
        GeoTransform  *xform4    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D((i)*cm, -(2*j+1)*cShift+downShift, 0));
        world->add(xform4);
        world->add(oPhys);
        
      }
    }
  }
  else {
    for (int j=0; j<(nLayers/2); j++){
      for (int i=0; i<(nTubes/2 +1)+j; i++){
        GeoTransform  *xform1    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(0*outerRadius+(i)*cm, -2*j*cShift+downShift, 0));
        world->add(xform1);
        world->add(oPhys);
        GeoTransform  *xform2    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(0*outerRadius-(i)*cm, -2*j*cShift+downShift, 0));
        world->add(xform2);
        world->add(oPhys);
        GeoTransform  *xform3    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(outerRadius-(i+1)*cm, -(2*j+1)*cShift+downShift, 0));
        world->add(xform3);
        world->add(oPhys);
        GeoTransform  *xform4    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(outerRadius+(i)*cm, -(2*j+1)*cShift+downShift, 0));
        world->add(xform4);
        world->add(oPhys);
      }
    }
  }
}

// fill only one layer of tubes (green)
void SPDKosoyGeoPlugin::fillLayer(GeoVPhysVol *world, GeoPhysVol *oPhys,
  double cShift, double outerRadius, int nTubes, int nLayers, double downShift, double angZ){
  
    bool isEven = (nTubes % 2 == 0) ? true : false;

  if (isEven){
    for (int j=0; j<(nLayers/2); j++){
      for (int i=0; i<(nTubes/2 +1)+j; i++){
        GeoTransform  *xform1    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(outerRadius+(i-1)*cm, downShift, 0));
        world->add(xform1);
        world->add(oPhys);
        GeoTransform  *xform2    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(outerRadius-i*cm, downShift, 0));
        world->add(xform2);
        world->add(oPhys);
      }
    }
  }
  else {
    for (int j=0; j<(nLayers/2); j++){
      for (int i=0; i<(nTubes/2 +1)+j; i++){
        GeoTransform  *xform1    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(0*outerRadius+(i)*cm, downShift, 0));
        world->add(xform1);
        world->add(oPhys);
        GeoTransform  *xform2    = new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(0*outerRadius-(i)*cm, downShift, 0));
        world->add(xform2);
        world->add(oPhys);
      }
    }
  }
}

// Trapezoid from red and blue tubes
void SPDKosoyGeoPlugin::fillTrapezoid2(GeoVPhysVol* world, GeoPhysVol* redPhys, GeoPhysVol* bluePhys,
  double cShift, double outerRadius, int nTubes, double downShift, double angleY, double angZ) {

  const int totalLayers = 4;  // Всего 4 слоя (2 красных, 2 синих)
  const double layerHeight = cShift + 2*outerRadius;

  // Конфигурация каждого слоя
  struct LayerConfig {
    int tubeReduction;  // Уменьшение количества трубок (nTubes - tubeReduction)
    double yBaseOffset; // Базовое смещение по Y
    bool isBlue;        // Цвет слоя
  };

  const LayerConfig layers[totalLayers] = {
    {0, 0.0, false},               // Слой 1 (красный)
    {2, layerHeight, true},         // Слой 2 (синий)
    {4, 2*layerHeight, false},      // Слой 3 (красный)
    {6, 3*layerHeight, true}        // Слой 4 (синий)
  };

  for (int layer = 0; layer < totalLayers; ++layer) {
    const auto& config = layers[layer];
    GeoPhysVol* currentPhys = config.isBlue ? bluePhys : redPhys;
    int tubesInLayer = (nTubes - config.tubeReduction) / 2;
    double angY = config.isBlue ? -angleY : angleY;
    
    for (int i = tubesInLayer; i > 0; --i) {
      // Координаты для основных элементов
      double x1 = outerRadius + (i-1)*cm;
      double x2 = outerRadius - i*cm;
      double baseY = config.yBaseOffset + downShift;

      // Координаты для центральных элементов
      double x3 = -(i-1)*cm;
      double x4 = (i-1)*cm;
      double centerY = config.yBaseOffset + cShift + downShift;

      // Добавляем все 4 элемента
      world->add(new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(x1, baseY, 0)*GeoTrf::RotateY3D(angY*degree)));
      world->add(currentPhys);

      world->add(new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(x2, baseY, 0)*GeoTrf::RotateY3D(angY*degree)));
      world->add(currentPhys);

      world->add(new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(x3, centerY, 0)*GeoTrf::RotateY3D(angY*degree)));
      world->add(currentPhys);

      world->add(new GeoTransform(GeoTrf::RotateZ3D(angZ*degree)*GeoTrf::Translate3D(x4, centerY, 0)*GeoTrf::RotateY3D(angY*degree)));
      world->add(currentPhys);
    }
  }
}


void SPDKosoyGeoPlugin::buildSextantB(GeoVPhysVol *world, GeoPhysVol *t1Phys,GeoPhysVol *tRedPhys, 
  GeoPhysVol *tBluePhys, double cShift, double outerRadius, double alphaY, double angZ){
  
  int nTubes = 32;
  double downShift0 = 33*cm*sqrt(3.)/2;

  int nLayers = 8;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift0, angZ );

  double downShift1 = downShift0+7*cShift+2*outerRadius;
  nTubes = 30;
  fillTrapezoid2(world, tRedPhys, tBluePhys,  cShift, outerRadius, nTubes, downShift1, alphaY,angZ);

  // nTubes  =56; //from last
  nTubes = 49;
  //skip red and blues tubes
  double downShift2 = downShift1 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;

  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift2,angZ );

  
  // Second red and blue layer (same as previous)
  double downShift3 = downShift2+7*cShift+2*outerRadius;
  nTubes = 64;
  fillTrapezoid2(world, tRedPhys, tBluePhys,  cShift, outerRadius, nTubes, downShift3, alphaY,angZ);
  

  // third green tubes layer
  double downShift4 = downShift3 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
  nTubes = 66;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift4,angZ);

  // third red and blue tubes layer
  double downShift5 = downShift4 + 7*cShift+2*outerRadius;
  nTubes = 63; // from first (56 from last)
  fillTrapezoid2(world, tRedPhys, tBluePhys,  cShift, outerRadius, nTubes, downShift5, alphaY,angZ);
  

  double downShift6 = downShift5 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
  nLayers = 6;
  nTubes = 83;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift6,angZ);

  //87, 84, 81, 76, 73, 68, 65, 60, 55, 50, 43, 34, 25 
  nLayers =2;
  nTubes = 87;
  double downShift7 = downShift6 + 6*cShift;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7, angZ);
  nTubes = 81;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+cShift, angZ);
  nTubes = 76;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+2*cShift,angZ);
  nTubes = 73;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+3*cShift,angZ);
  nTubes = 68;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+4*cShift,angZ);
  nTubes = 65;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+5*cShift,angZ);
  nTubes = 60;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+6*cShift,angZ);
  nTubes = 50;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+7*cShift,angZ);
  nTubes = 43;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+8*cShift,angZ);
  nTubes = 34;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+9*cShift,angZ);
  nTubes = 25;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+10*cShift,angZ);
  
}

void SPDKosoyGeoPlugin::buildSextantA(GeoVPhysVol *world, GeoPhysVol *t1Phys,GeoPhysVol *tRedPhys, 
  GeoPhysVol *tBluePhys, double cShift, double outerRadius, double alphaY, double angZ){
    

  int nTubes  = 34; // from first layer
  double downShift0 = 33*cm*sqrt(3.)/2;
  int nLayers = 8;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift0, angZ );

  double downShift1 = downShift0+7*cShift+2*outerRadius;
  nTubes = 47;
  fillTrapezoid2(world, tRedPhys, tBluePhys,  cShift, outerRadius, nTubes, downShift1, alphaY,angZ);

  // nTubes  =56; //from last
  nTubes = 51;
  //skip red and blues tubes
  double downShift2 = downShift1 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift2,angZ );

  // Second red and blue layer (same as previous)
  double downShift3 = downShift2+7*cShift+2*outerRadius;
  nTubes = 47;
  fillTrapezoid2(world, tRedPhys, tBluePhys,  cShift, outerRadius, nTubes, downShift3, alphaY,angZ);
  
  // third green tubes layer
  double downShift4 = downShift3 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
  nTubes = 68;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift4,angZ);

  // third red and blue tubes layer
  double downShift5 = downShift4 + 7*cShift+2*outerRadius;
  nTubes = 80;
  fillTrapezoid2(world, tRedPhys, tBluePhys,  cShift, outerRadius, nTubes, downShift5, alphaY,angZ);
  

  double downShift6 = downShift5 + (2*outerRadius+cShift+2*outerRadius+cShift)*2;
  nLayers = 6;
  nTubes = 85;
  fillTrapezoid(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift6,angZ);

  //87, 84, 81, 76, 73, 68, 65, 60, 55, 50, 43, 34, 25 
  nLayers =2;
  nTubes = 87;
  double downShift7 = downShift6 + 6*cShift;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7, angZ);
  nTubes = 81;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+cShift, angZ);
  nTubes = 76;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+2*cShift,angZ);
  nTubes = 73;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+3*cShift,angZ);
  nTubes = 68;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+4*cShift,angZ);
  nTubes = 65;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+5*cShift,angZ);
  nTubes = 60;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+6*cShift,angZ);
  nTubes = 50;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+7*cShift,angZ);
  nTubes = 43;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+8*cShift,angZ);
  nTubes = 34;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+9*cShift,angZ);
  nTubes = 25;
  fillLayer(world, t1Phys,-cShift, outerRadius, nTubes, nLayers, downShift7+10*cShift,angZ);

  }



// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SPDKosoyGeoPlugin *createSPDKosoyGeoPlugin() {
  return new SPDKosoyGeoPlugin;
}



