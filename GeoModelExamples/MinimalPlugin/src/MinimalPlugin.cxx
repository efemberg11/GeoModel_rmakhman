#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"
// Class Declaration

class MinimalPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  MinimalPlugin();

  // Destructor:
  ~MinimalPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish=false);

 private:

  // Illegal operations:
  const MinimalPlugin & operator=(const MinimalPlugin &right)=delete;
  MinimalPlugin(const MinimalPlugin &right) = delete;

};


// Class definition:

// Constructor
MinimalPlugin::MinimalPlugin()
{
}

// Destructor
MinimalPlugin::~MinimalPlugin()
{
}

// The create algorithm creates a tree of physical volumes rooted under the
// "world" physical volume. The optional flag publish is not used in this
// example (normally one may "publish" a list of FullPhysVol's and Alignable
// transforms, but this example has none such).
//
void MinimalPlugin::create(GeoPhysVol *world, bool /*publish*/)
{
  const double degree=M_PI/180.0;

  // Define elements used in this example:
  GeoElement  *aluminium     = new GeoElement("Aluminium", "Al", 13,  26 * GeoModelKernelUnits::g/GeoModelKernelUnits::mole);

  // Define materials:
  // Define Aluminium
  double densityOfAluminium=2.7;                  // g/cm^3
  GeoMaterial *Aluminium     = new GeoMaterial("Aluminium", densityOfAluminium);
  Aluminium->add(aluminium,1.0);
  Aluminium->lock();

   // Add the four legs of the kitchen sink:
  {
    const GeoBox       *b1        = new GeoBox(2,2,2);
    const GeoBox       *b2        = new GeoBox(.35,.35,.35);
    const GeoLogVol    *t1        = new GeoLogVol("B1",b1,Aluminium);
    const GeoLogVol    *t2        = new GeoLogVol("B2",b2,Aluminium);
    GeoPhysVol   *p1        = new GeoPhysVol(t1);
    GeoPhysVol   *p2        = new GeoPhysVol(t2);
    GeoTransform *x1     =new GeoTransform(GeoTrf::Translate3D(-0.5, 0, 0) );
    GeoTransform *x2     =new GeoTransform(GeoTrf::Translate3D(+0.5, 0, 0) );
    GeoTransform *X1      =new GeoTransform(GeoTrf::Translate3D(-4, 0, 0) );
    GeoTransform *X2      =new GeoTransform(GeoTrf::Translate3D(+4, 0, 0) );
    world->add(X1);
    world->add(p1);
    p1->add(x1);
    p1->add(p2);
    p1->add(x2);
    p1->add(p2);
    world->add(X2);
    world->add(p1);
  }
    

  //--------------------------------------//
}

// The name of this routine must correspond to the name of the class,
// and also to the name of the source code file (this file)

extern "C" MinimalPlugin *createMinimalPlugin() {
  return new MinimalPlugin;
}
