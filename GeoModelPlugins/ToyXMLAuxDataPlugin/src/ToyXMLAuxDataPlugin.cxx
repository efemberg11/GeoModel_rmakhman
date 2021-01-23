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
#include "GeoModelKernel/GeoPublisher.h" // to publish lists of FullPhysVol and AlignableTransform nodes and auxiliary data

#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/Sin.h"
#include "GeoGenericFunctions/Cos.h"

#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"


#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'SYSTEM_OF_UNITS::cm'

#include <memory>


using namespace GeoGenfun;
using namespace GeoXF;

class ToyXMLAuxDataPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor 
  ToyXMLAuxDataPlugin( std::string pluginName) : GeoVGeometryPlugin( pluginName ) {};

  // Destructor:
  ~ToyXMLAuxDataPlugin() {};

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish) override;

 
 private:

  // Illegal operations:
  const ToyXMLAuxDataPlugin & operator=(const ToyXMLAuxDataPlugin &right)=delete;
  ToyXMLAuxDataPlugin(const ToyXMLAuxDataPlugin &right) = delete;
 
};



//## Other Operations (implementation)
void ToyXMLAuxDataPlugin::create(GeoPhysVol *world, bool publish)
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

    // Publish GeoAlignableTransform and GeoFullPhysVol nodes, if a pointer to a GeoPublisher is provided
    if (publish) {
        
        // *** publish the list of FPV and AXF nodes ***
        // as an example, we use integer-based keys for FullPhysVol (FPV) nodes...
        unsigned int keyInt = i+1;
        m_publisher->publishNode<GeoVFullPhysVol*,unsigned >( ringPhys, keyInt );
        
        // ...and string-based keys for AlignableTransform (AXF) nodes
        std::string keyStr = this->getName() + "-AXF-" + std::to_string(i+1);
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
 


  //=====================
  // Auxiliary Data
  //=====================

  // Now, we save auxiliary data to the output DB file
  // in additional custom DB tables
  // Those data are meant to be used by detector code later in the workflow
  // so, it must be stored in the DB besides the actual GeoModel data themselves.
  //

  // In these first two examples, we load auxiliary data from custom XML files
  // through the use of the GeoModelATLAS/GeoModelDataManagers
  // Then, we store the auxiliary data directly to the DB files.
  //
  // At first, we need to get the GeoModelDataManagers' XML Input Manager
  // As per GeoModelDataManagers' architecture, we need to load both the file
  // containing the tables' defiintions and the one containing the actual tables' data
  GeoXmlInpManager* inpman = GeoXmlInpManager::getManager();
  inpman->parse("Hello-Defs.xml");
  inpman->parse("Hello-Data.xml");
  // Then, we set the name of the first of the tables that we want to store in the DB...
  std::string tableName = "HelloCables";
  // ...we get the corresponding "Recordset"'s data (i.e., the table's columns definitions and data rows
  std::pair<std::map<std::string, std::vector<std::string>>, std::vector<std::vector<GeoInp>>> helloCablesData = inpman->getRecordsetData(tableName);
  // ...and we store that table to the output DB
  m_publisher->storeDataTable( tableName, (helloCablesData.first)["colNames"], (helloCablesData.first)["colTypes"], helloCablesData.second );

  // Here an example for a second table of auxiliary data, which we want to store in the DB for later use
  tableName = "HelloBoxes";
  std::pair<std::map<std::string, std::vector<std::string>>, std::vector<std::vector<GeoInp>>> helloBoxesData = inpman->getRecordsetData(tableName);
  m_publisher->storeDataTable( tableName, (helloBoxesData.first)["colNames"], (helloBoxesData.first)["colTypes"], helloBoxesData.second );

  // Not all auxiliary data need to come from XML files.
  // Here, we create a custom table made of strings, directly in the code.
  // We define a table's name
  // Then, we define the names and types of the columns, storing them as vectors of strings
  // Then, we populate vectors of strings with the table's data. One vector for each table's row.
  // Then, we store the data into the DB.
  tableName = "HelloTest";
  std::vector<std::string> tableColNames = {"MY_DATA_ID", "WEIGTH", "NAME"};
  std::vector<std::string> tableColTypes = {"long", "double", "string"};
  std::vector<std::variant<int,long,float,double,std::string>> vec1 = {1, 15, "Ped1"};
  std::vector<std::variant<int,long,float,double,std::string>> vec2 = {3, "NULL", "Crate2"};
  std::vector<std::vector<std::variant<int,long,float,double,std::string>>> tableData = { vec1, vec2 };
  m_publisher->storeDataTable( tableName, tableColNames, tableColTypes, tableData );


  return;
}

extern "C" ToyXMLAuxDataPlugin *createToyXMLAuxDataPlugin() {
  ToyXMLAuxDataPlugin* toy = new ToyXMLAuxDataPlugin( "ToyXMLAuxDataPlugin" );
  std::cout << "The plugin, whose name is '" << toy->getName() << "', has been created." << std::endl;
  return toy;
}
