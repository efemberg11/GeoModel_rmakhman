/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"

#include "AGDDControl/AGDDController.h"
#include "AGDDKernel/AGDDSectionStore.h"
#include "AGDDControl/AGDD2GeoModelBuilder.h"

#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

#include <iostream>

class AGDDPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  AGDDPlugin();

  // Destructor:
  ~AGDDPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world);

 private:

  // Illegal operations:
  const AGDDPlugin & operator=(const AGDDPlugin &right)=delete;
  AGDDPlugin(const AGDDPlugin &right) = delete;

};





AGDDPlugin::AGDDPlugin()
{
}


AGDDPlugin::~AGDDPlugin()
{
}


//## Other Operations (implementation)
void AGDDPlugin::create(GeoPhysVol *world)
{

  	std::cout<< "creating an AGDDController "<<std::endl;
  	AGDDController*  control=AGDDController::GetController();
	AGDD2GeoModelBuilder* builder=dynamic_cast<AGDD2GeoModelBuilder*>(control->GetBuilder());
	if (builder) builder->SetMotherVolume(world);
	control->AddFile("AMDC.xml");
	control->ParseFiles();

  	std::cout<<" printing sections "<<std::endl;
  	control->PrintSections();
	
	control->BuildAllSections();
}

extern "C" AGDDPlugin *createAGDDPlugin() {
  return new AGDDPlugin;
}
