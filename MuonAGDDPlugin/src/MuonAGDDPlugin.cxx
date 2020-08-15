/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"

#include "AGDDControl/AGDDController.h"
#include "AGDDKernel/AGDDSectionStore.h"
#include "AGDDControl/AGDD2GeoModelBuilder.h"

#include "MuonAGDDBase/micromegasHandler.h"
#include "MuonAGDDBase/mm_TechHandler.h"
#include "MuonAGDDBase/sTGCHandler.h"
#include "MuonAGDDBase/sTGC_readoutHandler.h"
#include "MuonAGDDBase/sTGC_TechHandler.h"
#include "MuonAGDDBase/mm_readoutHandler.h"

#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

#include <iostream>

#include "GeoXmlMatManager/GeoXmlMatManager.h" // needed since GeoXmlMatManager is forward declared in AGDD2GeoModelBuilder.h

class MuonAGDDPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  MuonAGDDPlugin();

  // Destructor:
  ~MuonAGDDPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, GeoVStore* store);

 private:

  // Illegal operations:
  const MuonAGDDPlugin & operator=(const MuonAGDDPlugin &right)=delete;
  MuonAGDDPlugin(const MuonAGDDPlugin &right) = delete;

};





MuonAGDDPlugin::MuonAGDDPlugin()
{
}


MuonAGDDPlugin::~MuonAGDDPlugin()
{
}


//## Other Operations (implementation)
void MuonAGDDPlugin::create(GeoPhysVol *world, GeoVStore*)
{

  	std::cout<< "MuonAGDDPlugin: creating an AGDDController "<<std::endl;
  	AGDDController*  control=AGDDController::GetController();
	
	std::cout<<"MuonAGDDPlugin: adding NSW-related handlers "<<std::endl;
	
	control->addHandler(new micromegasHandler("micromegas"));
        control->addHandler(new mm_TechHandler("mm_Tech"));
        control->addHandler(new sTGCHandler("sTGC"));
        control->addHandler(new sTGC_readoutHandler("sTGC_readout"));
        control->addHandler(new sTGC_TechHandler("sTGC_Tech"));
        control->addHandler(new mm_readoutHandler("mm_readout"));
	
	AGDD2GeoModelBuilder* builder=dynamic_cast<AGDD2GeoModelBuilder*>(control->GetBuilder());
	if (builder) builder->SetMotherVolume(world);
	control->AddFile("AMDC.xml");
	control->ParseFiles();

//  	std::cout<<" printing sections "<<std::endl;
//  	control->PrintSections();
	
	control->BuildAllSections();
}

extern "C" MuonAGDDPlugin *createMuonAGDDPlugin() {
  return new MuonAGDDPlugin;
}
