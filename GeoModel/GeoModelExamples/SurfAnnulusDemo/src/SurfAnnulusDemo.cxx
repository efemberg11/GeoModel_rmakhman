/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

// -------------------------------------------------------------------
//
// Annulus Surface Demo Plugin
// Rui Xue, July 2024
//
//
// This plugin is used to test and develop Annulus Surface
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

class SurfAnnulusDemo : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  SurfAnnulusDemo();

  // Destructor:
  ~SurfAnnulusDemo();

  // Creation of geometry:
  virtual void create(GeoVPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const SurfAnnulusDemo & operator=(const SurfAnnulusDemo &right)=delete;
  SurfAnnulusDemo(const SurfAnnulusDemo &right) = delete;

};


// Class definition:

// Constructor
SurfAnnulusDemo::SurfAnnulusDemo()
{
}

// Destructor
SurfAnnulusDemo::~SurfAnnulusDemo()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void SurfAnnulusDemo::create(GeoVPhysVol *world, bool /*publish*/) {
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

    GeoAnnulusSurface* annulus = new GeoAnnulusSurface(-4.0, -5.0, 7.0, 10.0, 0.3*M_PI); // Ox, Oy, Rin, Rout, Phi
    GeoVSurface* surf3 = new GeoVSurface(annulus);
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

          //GeoFullPhysVol *point_phys2 = new GeoFullPhysVol(point_log);
          //GeoTransform * translate_point2 = new GeoTransform(GeoTrf::TranslateX3D(test_x)*GeoTrf::TranslateY3D(test_y)*GeoTrf::TranslateZ3D(test_z));
          //boxPhys->add(translate_point2);
          //boxPhys->add(point_phys2);

          if (surf3->isOnSurface(test_x, test_y, test_z))
          { 
            point_phys2 = new GeoFullPhysVol(point_log); 
            std::cout << "  the point is on the surface " << std::endl;
          }
          else{
            point_phys2 = new GeoFullPhysVol(point_log2);
            std::cout << " NOT ON THE SURFACE " << std::endl;
          }
          translate_point2 = new GeoTransform(GeoTrf::TranslateX3D(test_x)*GeoTrf::TranslateY3D(test_y)*GeoTrf::TranslateZ3D(test_z));
          boxPhys->add(translate_point2);
          boxPhys->add(point_phys2);            
        //}
      }
    }

  }
  
    // GeoTrapezoidSurface* trapezoid2 = new GeoTrapezoidSurface(9, 12, 8);
    // GeoVSurface* surf5 = new GeoVSurface(trapezoid2);
    // GeoAlignableTransform *move_together5=new GeoAlignableTransform(GeoTrf::TranslateZ3D(20)*GeoTrf::TranslateX3D(40));
    
    // boxPhys->add(move_together5);
    // boxPhys->add(surf5);
    // move_together5->setDelta(GeoTrf::RotateX3D(1));
    
  world->add(boxPhys);
  GeoPrintGraphAction printGraphAction(std::cout);
  world->exec(&printGraphAction);;

  {
    int i = 0;
    
    // std::cout << " " << std::endl;
    // std::cout << " " << std::endl;    
    // std::cout << " cursor at i= " << i << std::endl;
    GeoVolumeCursor cursor(boxPhys);
    //GeoSurfaceCursor cursor(boxPhys);
    
    while (!cursor.atEnd()) {
      i += 1;
      // std::cout << "!!! " << std::endl;
      // std::cout << cursor.getTransform().rotation() << std::endl;
      // std::cout << "!!! " << std::endl; 
      // std::cout << cursor.getTransform().translation() << std::endl;
      // std::cout << " " << std::endl;
      cursor.next();
    }
  }

  GeoCountSurfaceAction countSurface;
  boxPhys->exec(&countSurface);
  // std::cout << "Number of Virtual Surface: " << countSurface.getCount() << std::endl;
  unsigned int totalVSurface = countSurface.getCount();
  
  for (unsigned int i = 0; i< totalVSurface; i++){
    GeoAccessSurfaceAction surfaceAccess (i, nullptr);
    boxPhys->exec(&surfaceAccess);
    // std::cout << "surface transform rotation: " << surfaceAccess.getTransform().rotation() << std::endl;
    // std::cout << "surface transform trans: " << surfaceAccess.getTransform().translation() << std::endl;
  }
   
  //  // cannot use GeoNodePositioning::getAbsoluteTransform(). Is there an error in the method?
  //  std::cout << "surface5 transform rot: " << surf5->getX().rotation() << std::endl;
  //  std::cout << " " << std::endl;
  //  //surf5->PrintRotation();
   
  //  GeoTrf::Vector3D vec(0.0, 0.0, 1.0);
  //  std::cout << " ROTATE VECTOR " << surf5->getX().rotation()*vec << std::endl;
   
  //  std::cout << " " << std::endl;
  //  std::cout << " " << std::endl;
  //  //surf5->PrintTranslation();
  //  std::cout << "surface5 transform trans: " << surf5->getX().translation() << std::endl;
   
  //  Eigen::Matrix4d M_4d = Eigen::Matrix4d::Identity();
  //  M_4d.block<3,3>(0,0) = surf5->getX().rotation();
  //  M_4d.block<3,1>(0,3) = surf5->getX().translation();
  //  std::cout << " 4x4 matrix " << M_4d << std::endl;
   
  //  Eigen::Vector4d vec_4d;
  //  vec_4d << vec, 1.0;
  //  //Eigen::Vector4d trans_vec_4d = M_4d * vec_4d;
  //  Eigen::Vector4d trans_vec_4d = surf5->getX() * vec_4d;
  //  Eigen::Vector3d trans_vec = trans_vec_4d.head<3>();
   
  //  Eigen::Vector4d origin_vec_4d = surf5->getX().inverse() * trans_vec_4d;
  //  Eigen::Vector3d origin_vec = origin_vec_4d.head<3>();   
  //  std::cout << " TRANS VECTOR " << trans_vec << std::endl;
  //  std::cout << " ORIGIN VECTOR " << origin_vec << std::endl;

  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" SurfAnnulusDemo *createSurfAnnulusDemo() {
  return new SurfAnnulusDemo;
}

