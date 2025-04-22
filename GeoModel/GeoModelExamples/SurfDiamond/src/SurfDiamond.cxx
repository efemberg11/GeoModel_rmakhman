/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// Diamond Surface Demo Plugin
// Rui Xue, Oct 2024
//
//
// This plugin is used to test and develop Diamond Surface
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
#include "GeoModelKernel/GeoAnnulusSurface.h"
#include "GeoModelKernel/GeoDiamondSurface.h"
#include "GeoModelKernel/GeoVSurface.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelHelpers/defineWorld.h"

#include "GeoModelKernel/GeoSurfaceCursor.h"
#include "GeoModelKernel/GeoCountSurfaceAction.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoAccessSurfaceAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <iostream>
#include <cmath>
using namespace GeoModelKernelUnits;

// Class Declaration

class SurfDiamond : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  SurfDiamond();

  // Destructor:
  ~SurfDiamond();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const SurfDiamond & operator=(const SurfDiamond &right)=delete;
  SurfDiamond(const SurfDiamond &right) = delete;

};


// Class definition:

// Constructor
SurfDiamond::SurfDiamond()
{
}

// Destructor
SurfDiamond::~SurfDiamond()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void SurfDiamond::create(GeoVPhysVol *world, bool /*publish*/) {
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
    GeoBox    *point_box = new GeoBox(0.2, 0.2, 0.2);
    GeoLogVol *point_log = new GeoLogVol("PointLog", point_box, Air);
    GeoBox    *point_box2 = new GeoBox(0.05, 0.05, 0.05);
    GeoLogVol *point_log2 = new GeoLogVol("PointLog", point_box2, Air);

    GeoFullPhysVol *point_phys = new GeoFullPhysVol(point_log);

    // The point initial position, definitely on the initial surface
    // after a series of transformations, the point should still be on the surface
    // this part is to test the GeoVSurface::isOnSurface method
    // it should return true
    const double point_x =  5.66; 
    const double point_y = -5.66; 
    const double point_z = 0.0;
    Eigen::Vector4d point_vec(point_x, point_y, point_z, 1.0);
    Eigen::Vector4d origin_p(0.0, 0.0, 0.0, 1.0);
    Eigen::Vector4d trans_point_vec;
    Eigen::Vector4d trans_origin_p0;
    Eigen::Vector4d trans_origin_p;

    GeoTransform * transform_point = new GeoTransform(GeoTrf::RotateY3D(0.4*M_PI)*GeoTrf::TranslateX3D(20)*GeoTrf::TranslateX3D(5.66)*GeoTrf::TranslateY3D(-5.66));

    GeoBox     *boxShape = new GeoBox(3,3,3);
    GeoLogVol  *boxLog = new GeoLogVol("BoxLog",boxShape,Air);
    GeoFullPhysVol *boxPhys3 = new GeoFullPhysVol(boxLog);

    // GeoAnnulusSurface* annulus = new GeoAnnulusSurface(-4.0, -5.0, 7.0, 10.0, 2*M_PI); // Ox, Oy, Rin, Rout, Phi
    // GeoVSurface* surf3 = new GeoVSurface(annulus);
    GeoDiamondSurface* diamond = new GeoDiamondSurface(4, 9.5, 7, 5.5, 6);
    GeoVSurface* surf3 = new GeoVSurface(diamond);
    // GeoRectSurface* rect = new GeoRectSurface(5, 9);
    // GeoVSurface* surf3 = new GeoVSurface(rect);
    // GeoTrapezoidSurface* trapezoid = new GeoTrapezoidSurface(9, 7, 5);
    // GeoVSurface* surf3 = new GeoVSurface(trapezoid);

    GeoAlignableTransform *move_together3=new GeoAlignableTransform(GeoTrf::RotateY3D(0.4*M_PI)*GeoTrf::TranslateX3D(20));
    //GeoTransform * point_trans = new GeoTransform(GeoTrf::RotateY3D(0.4*M_PI)*GeoTrf::TranslateX3D(20));
    boxPhys->add(move_together3);   
    boxPhys->add(surf3);
    // boxPhys->add(transform_point); // move the point (initially at (0,0,0)) to the starting position (5.66, -5.66, 0)
    // boxPhys->add(point_phys);
    move_together3->setDelta(GeoTrf::RotateY3D(0));
    
    GeoFullPhysVol *point_phys2;
    GeoTransform * translate_point2;

    for(int i = 0; i < 11; i++){
      for(int j = 0; j < 11; j++){
        //for(int k = 0; k < 3; k++){
          double test_x =  -10.0 + i*2;
          double test_y =  -10.0 + j*2; 
          double test_z =  0.0;//-1.0  + k*1.0;
          Eigen::Vector4d test_point(test_x, test_y, test_z, 1.0);
          test_point = surf3->getX() * test_point;
          test_x = test_point[0]; test_y = test_point[1]; test_z = test_point[2];

          if (surf3->isOnSurface(test_x, test_y, test_z))
          { 
            point_phys2 = new GeoFullPhysVol(point_log); 
            // std::cout << "  the point is on the surface " << std::endl;
          }
          else{
            point_phys2 = new GeoFullPhysVol(point_log2);
            // std::cout << " NOT ON THE SURFACE " << std::endl;
          }
          translate_point2 = new GeoTransform(GeoTrf::TranslateX3D(test_x)*GeoTrf::TranslateY3D(test_y)*GeoTrf::TranslateZ3D(test_z));
          boxPhys->add(translate_point2);
          boxPhys->add(point_phys2);            
        //}
      }
    }

  }
    
  world->add(boxPhys);
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SurfDiamond *createSurfDiamond() {
  return new SurfDiamond;
}

