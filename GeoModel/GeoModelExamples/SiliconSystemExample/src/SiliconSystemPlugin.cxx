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
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/GeoTrapezoidSurface.h"
#include "GeoModelKernel/GeoVSurface.h"
#include <random>
using namespace GeoModelKernelUnits;
std::mt19937 engine;
// Class Declaration
const double degrees=M_PI/180.0;
class SiliconSystemPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  SiliconSystemPlugin();

  // Destructor:
  ~SiliconSystemPlugin();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const SiliconSystemPlugin & operator=(const SiliconSystemPlugin &right)=delete;
  SiliconSystemPlugin(const SiliconSystemPlugin &right) = delete;

};


// Class definition:

// Constructor
SiliconSystemPlugin::SiliconSystemPlugin()
{
}

// Destructor
SiliconSystemPlugin::~SiliconSystemPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void SiliconSystemPlugin::create(GeoVPhysVol *world, bool /*publish*/) {
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

  std::normal_distribution<double> normal;
  
  // Define Aluminium
  double densityOfAluminium=2.7*gram/cm3;                  // g/cm^3
  GeoMaterial *Aluminium     = new GeoMaterial("Aluminium", densityOfAluminium);
  Aluminium->add(aluminium,1.0);
  Aluminium->lock();

  GeoBox     *boxShape = new GeoBox(10,10,10);
  GeoLogVol  *boxLog = new GeoLogVol("BoxLog",boxShape,Air);
  GeoPhysVol *worldBOX = new GeoPhysVol(boxLog);

  GeoBox *box1 = new GeoBox (0.1,1*1.7,20);
  GeoLogVol  *boxLog1 = new GeoLogVol("SiDet", box1, Aluminium);

  GeoBox *box2 = new GeoBox (0.1,2*1.7,20);
  GeoLogVol  *boxLog2 = new GeoLogVol("SiDet", box2, Aluminium);

  GeoBox *box3 = new GeoBox (0.1,3*1.7,20);
  GeoLogVol  *boxLog3 = new GeoLogVol("SiDet", box3, Aluminium);    

  GeoRectSurface* rectSurface1 = new GeoRectSurface(1*1.9, 20.2);
  GeoRectSurface* rectSurface2 = new GeoRectSurface(2*1.9, 20.2);
  GeoRectSurface* rectSurface3 = new GeoRectSurface(3*1.9, 20.2);

  for (int k=0;k<3;k++) {
    for (int j=0;j<3;j++)  {
      GeoLogVol* boxLog;
      if(j==0) boxLog = boxLog1;
      else if(j==1) boxLog = boxLog2;
      else boxLog = boxLog3;

      GeoRectSurface* rectSurface;
      if(j==0) rectSurface = rectSurface1;
      else if(j==1) rectSurface = rectSurface2;
      else rectSurface = rectSurface3;
      
      for (int i=0;i<16;i++) {
        double theta = i/16.0*2*M_PI;
        GeoFullPhysVol *boxPhys=new GeoFullPhysVol(boxLog);
        GeoVSurface* surf = new GeoVSurface(rectSurface);
        // Initial transform is very tricky. Because the Virtual surface is always facing to Z axis initially.
        GeoAlignableTransform* xf0 = new GeoAlignableTransform(GeoTrf::RotateZ3D(theta)*GeoTrf::TranslateX3D((j+1)*8.0+0.4*(i%2))*GeoTrf::TranslateX3D(0.5)*GeoTrf::TranslateZ3D((k-1)*44.0)*GeoTrf::RotateX3D(M_PI/2.0)*GeoTrf::RotateY3D(M_PI/2.0));
        GeoAlignableTransform *xf = new GeoAlignableTransform(GeoTrf::RotateZ3D(theta)*GeoTrf::TranslateX3D((j+1)*8.0+0.4*(i%2)));

        double rndTheta=normal(engine)*5*degrees;
        double rndPhi=normal(engine)*5*degrees;
        double rndPsi=normal(engine)*5*degrees;

        // The Delta Setting is really tricky. The local axis are different because of different initialization order.
        Eigen::Matrix3d m;
        m = Eigen::AngleAxisd(rndTheta, Eigen::Vector3d::UnitZ())
          * Eigen::AngleAxisd(rndPhi,   Eigen::Vector3d::UnitY())
          * Eigen::AngleAxisd(rndPsi,   Eigen::Vector3d::UnitZ());

        Eigen::Matrix3d m0;
        m0 = Eigen::AngleAxisd(rndTheta, Eigen::Vector3d::UnitY())
           * Eigen::AngleAxisd(rndPhi,   Eigen::Vector3d::UnitX())
           * Eigen::AngleAxisd(rndPsi,   Eigen::Vector3d::UnitY());
        
        xf0->setDelta(GeoTrf::Transform3D(m0));
        xf->setDelta(GeoTrf::Transform3D(m));
        worldBOX->add(new GeoTransform(GeoTrf::TranslateZ3D((k-1)*44.0)));
                
        worldBOX->add(xf);
        worldBOX->add(boxPhys);

        worldBOX->add(xf0);
        worldBOX->add(surf);
      }
    }
    
  }
  double L=10.0;
  GeoTrapezoidSurface* trapezoid = new GeoTrapezoidSurface(1.2, 5.2, L); 
  GeoTrd *trd=new GeoTrd(.2, .2, 1, 5, L);
  GeoLogVol *trdLog=new GeoLogVol("SiDetEnd", trd,Aluminium);

  for (int j=0;j<16;j++) {
    for (int i=-1;i<2;i+=2)  {     
      GeoVSurface* surf = new GeoVSurface(trapezoid);
      GeoFullPhysVol *trdPhys=new GeoFullPhysVol(trdLog);

      GeoTransform *sf0=new GeoTransform(GeoTrf::RotateY3D(j*2*M_PI/16.0)*GeoTrf::RotateX3D(M_PI/2.0)*GeoTrf::TranslateY3D(1.5*L));
      GeoAlignableTransform* sf1 = nullptr;
      if(i==-1){
        std::cout << "i = -1" << std::endl;
        sf1 = new GeoAlignableTransform(GeoTrf::TranslateZ3D(i*80.0+2.0*(j%2))*GeoTrf::TranslateZ3D(-1.0)*GeoTrf::RotateX3D(M_PI/2.0)*GeoTrf::RotateY3D(M_PI/2.0));
      }
      else{
        std::cout << "i = 1" << std::endl;
        sf1 = new GeoAlignableTransform(GeoTrf::TranslateZ3D(i*80.0+2.0*(j%2))*GeoTrf::TranslateZ3D(1.0)*GeoTrf::RotateX3D(M_PI/2.0)*GeoTrf::RotateY3D(M_PI/2.0));
      }
      
      GeoTransform *xf0=new GeoTransform(GeoTrf::RotateZ3D(j*2*M_PI/16.0)*GeoTrf::RotateY3D(M_PI/2.0)*GeoTrf::TranslateZ3D(1.5*L));
      GeoAlignableTransform *xf1= new GeoAlignableTransform (GeoTrf::TranslateZ3D(i*80.0+2.0*(j%2)));
     
      
      worldBOX->add(xf1);
      worldBOX->add(xf0);
      worldBOX->add(trdPhys);
      worldBOX->add(sf1);
      worldBOX->add(sf0);
      worldBOX->add(surf);
    }
  }
  world->add(worldBOX);
    //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SiliconSystemPlugin *createSiliconSystemPlugin() {
  return new SiliconSystemPlugin;
}

