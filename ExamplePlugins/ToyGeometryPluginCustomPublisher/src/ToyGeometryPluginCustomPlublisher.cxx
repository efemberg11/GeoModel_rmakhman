/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoSerialDenominator.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoPublisher.h" // to publish lists of FullPhysVol and AlignableTransform nodes

#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/Sin.h"
#include "GeoGenericFunctions/Cos.h"

#include "GeoModelKernel/Units.h"

#include "GeoXmlMatManager/GeoXmlMatManager.h"

#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'SYSTEM_OF_UNITS::cm'

using namespace GeoGenfun;
using namespace GeoXF;

class ToyGeometryPluginCustomPublisher : public GeoVGeometryPlugin  {

 public:

  // Constructor
  ToyGeometryPluginCustomPublisher( std::string pluginName) : GeoVGeometryPlugin( pluginName ) {};

  // Destructor:
  ~ToyGeometryPluginCustomPublisher();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish) override;

 
 private:

  // Illegal operations:
  // we prohibit default constructor, copy constructor, and assignment operator
  const ToyGeometryPluginCustomPublisher & operator=(const ToyGeometryPluginCustomPublisher &right)=delete;
  ToyGeometryPluginCustomPublisher(const ToyGeometryPluginCustomPublisher &right) = delete;

};


ToyGeometryPluginCustomPublisher::~ToyGeometryPluginCustomPublisher()
{
}


//## Other Operations (implementation)
void ToyGeometryPluginCustomPublisher::create(GeoPhysVol *world, bool publish)
{
  // Get the materials that we shall use.
  // -------------------------------------//
  const GeoXmlMatManager* matman = GeoXmlMatManager::getManager();
  
  const GeoMaterial *air        = matman->getMaterial("std::Air");
  const GeoMaterial *poly       = matman->getMaterial("std::Polystyrene");
  const GeoMaterial *silicon    = matman->getMaterial("std::Silicon");
  const GeoMaterial *copper     = matman->getMaterial("std::Copper");
 
  //--------------------------------------//
  // Next make the box that describes
  // the shape of the toy volume:
  //--------------------------------------//
  const GeoBox *toyBox = new GeoBox(1200*SYSTEM_OF_UNITS::cm,1200*SYSTEM_OF_UNITS::cm, 1200*SYSTEM_OF_UNITS::cm);
  
  //--------------------------------------//
  // Bundle this with a material
  // into a logical volume:
  //--------------------------------------//
  const GeoLogVol *toyLog = new GeoLogVol("ToyLog", toyBox, air);                 //
  
  //--------------------------------------//
  // ..And create a physical volume:
  //--------------------------------------//
  GeoPhysVol *toyPhys = new GeoPhysVol(toyLog);
  
  //--------------------------------------//
  // Daughters
  //--------------------------------------//
  const GeoTube *ringTube  = new GeoTube(500*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 5.0*SYSTEM_OF_UNITS::cm);
  
  // Bundle this with a material //
  // into a logical volume:      //
  const GeoLogVol   *ringLog  = new  GeoLogVol("RingLog", ringTube, copper);
  
  // Make 100 of these              //
  // within the volume of the toy:  //
  GeoSerialDenominator *ringName = new GeoSerialDenominator("RING");
  toyPhys->add(ringName);
 
  for (int i=0;i<100;i++) {
    GeoFullPhysVol         *ringPhys = new GeoFullPhysVol(ringLog);
    GeoAlignableTransform  *xform    = new GeoAlignableTransform(GeoTrf::TranslateZ3D((i-50)*20*SYSTEM_OF_UNITS::cm));
    toyPhys->add(xform);
    toyPhys->add(ringPhys);

    // publish GeoAlignableTransform and GeoFullPhysVol nodes, if a pointer to a GeoPublisher is provided
    if ( publish ) {
        
        // *** publish the list of FPV and AXF nodes ***
        // as an example, we use string-based keys for FullPhysVols...
        unsigned int keyInt = i+1;
        m_publisher->publishNode<GeoVFullPhysVol*,unsigned>( ringPhys, keyInt );
        
        // ...and integer-based keys for AlignableTransforms
        std::string keyStr = "Toy-AXF-" + std::to_string(i+1);
        m_publisher->publishNode<GeoAlignableTransform*,std::string>( xform, keyStr );
    }
  }


  //--------------------------------------//
  // Now, in addition to active daughters,
  // add some passive material.
  // This is done here using
  // the "SerialTransformer",
  // our way of parameterizing volumes.
  // It does not need to be done this way,
  // but we want to provide an example of
  // parametrizations in the Toy
  //--------------------------------------//

  GeoBox       *sPass = new GeoBox(5.0*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm);
  GeoLogVol    *lPass = new GeoLogVol("Passive", sPass, poly);
  GeoPhysVol   *pPass = new GeoPhysVol(lPass);

  GeoBox       *sIPass = new GeoBox(4*SYSTEM_OF_UNITS::cm, 25*SYSTEM_OF_UNITS::cm, 25*SYSTEM_OF_UNITS::cm);
  GeoLogVol    *lIPass = new GeoLogVol("InnerPassive", sIPass, silicon);
  GeoPhysVol   *pIPass = new GeoPhysVol(lIPass);

  pPass->add(pIPass);

  const unsigned int NPLATES=100;
  Variable       i;
  Sin            sin;
  GENFUNCTION    f = 360*SYSTEM_OF_UNITS::deg/NPLATES*i;
  GENFUNCTION    g = sin(4*f);
  GENFUNCTION    h = -g;
  TRANSFUNCTION t1 = Pow(GeoTrf::RotateZ3D(1.0),f)*GeoTrf::TranslateX3D(1100*SYSTEM_OF_UNITS::cm)*Pow(GeoTrf::TranslateZ3D(800*SYSTEM_OF_UNITS::cm),g);
  TRANSFUNCTION t2 = Pow(GeoTrf::RotateZ3D(1.0),f)*GeoTrf::TranslateX3D(1100*SYSTEM_OF_UNITS::cm)*Pow(GeoTrf::TranslateZ3D(800*SYSTEM_OF_UNITS::cm),h);

  //--------------------------------------//
  // Inside, by the way, the serial transformer
  // will evaluate the functions:
  // HepTransform3D xf = t1(i), for i=1,NPLATES....
  //--------------------------------------//

  GeoSerialDenominator  *pass1Name = new GeoSerialDenominator("PASSIVE-1-");
  GeoSerialTransformer *s1 = new GeoSerialTransformer(pPass,&t1, NPLATES);
  toyPhys->add(pass1Name);
  toyPhys->add(s1);

  GeoSerialDenominator *pass2Name = new GeoSerialDenominator("PASSIVE-2-");
  GeoSerialTransformer *s2 = new GeoSerialTransformer(pPass,&t2, NPLATES);
  toyPhys->add(pass2Name);
  toyPhys->add(s2);


  //--------------------------------------//
  // Now insert all of this into the world...
  //--------------------------------------//
  GeoNameTag *tag = new GeoNameTag("Toy");
  world->add(tag);
  world->add(toyPhys);
  //--------------------------------------//
  
  return;
}

extern "C" ToyGeometryPluginCustomPublisher *createToyGeometryPluginCustomPublisher() {
  ToyGeometryPluginCustomPublisher* toy = new ToyGeometryPluginCustomPublisher( "ToyGeometryPluginCustomPublisher" );
  std::cout << "The plugin, whose name is '" << toy->getName() << "', has been created." << std::endl;
  return toy;
}
