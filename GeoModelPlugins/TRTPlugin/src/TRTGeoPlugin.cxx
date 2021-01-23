/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/


#include "TRTDetectorFactory_Full.h"

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoPhysVol.h"

#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"

#include <iostream>
#include <chrono>

#define SYSTEM_OF_UNITS GeoModelKernelUnits

class TRTGeoPlugin : public GeoVGeometryPlugin
{
public:
  TRTGeoPlugin()
    : GeoVGeometryPlugin()
  {}
  TRTGeoPlugin(std::string name)
    :GeoVGeometryPlugin(name)
  {}
  ~TRTGeoPlugin();

  virtual void create(GeoPhysVol* world, bool publish) override;

private:  
  const TRTGeoPlugin & operator=(const TRTGeoPlugin &right)=delete;
  TRTGeoPlugin(const TRTGeoPlugin &right) = delete;
  
};

TRTGeoPlugin::~TRTGeoPlugin()
{
}

void TRTGeoPlugin::create(GeoPhysVol* world, bool publish)
{
  // ________ Time measure ____________
  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::milliseconds ms;
  typedef std::chrono::duration<float> fsec;
  auto t0 = Time::now();
  // ________ Time measure ____________
    
  // Get XML Input Manager
  GeoXmlInpManager* inpman = GeoXmlInpManager::getManager();
  inpman->parse("TRT-Def.xml");
  inpman->parse("TRT-GEO-02.xml");

  // Get Configuration Parameters
  GeoInpRecordset_ptr switchSet =  inpman->getRecordsetPtr("TRTSwitches");
  const GeoInpRecord& switches  = (*switchSet)[0];
  
  bool useOldActiveGasMixture   = (switches.getInt("GASVERSION") == 0);
  bool DC2CompatibleBarrelCoordinates = switches.getInt("DC2COMPATIBLE");
  bool doArgonMixture = switches.getInt("DOARGONMIXTURE");
  bool doKryptonMixture = switches.getInt("DOKRYPTONMIXTURE");
  
  TRTDetectorFactory_Full factory(useOldActiveGasMixture
				  , DC2CompatibleBarrelCoordinates
				  , doArgonMixture
				  , doKryptonMixture);
  
  factory.create(world);
  
  // ________ Time measure ____________
  auto t1 = Time::now();
  fsec fs = t1 - t0;
  ms d = std::chrono::duration_cast<ms>(fs);
  std::cout << "TRTGeoPlugin elapsed time " << d.count() << "ms" << std::endl;
  // ________ Time measure ____________
}

extern "C" TRTGeoPlugin *createTRTGeoPlugin() {
  return new TRTGeoPlugin("TRT");
}
