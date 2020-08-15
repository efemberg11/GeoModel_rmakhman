/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

// local includes
#include <GeoModelPublish/GeoStore.h>

// C++ includes
#include <iostream>


namespace GeoModelIO {


void GeoStore::storeAXF(GeoAlignableTransform* axf, std::any key)
{
  std::cout << "AXF key: " << std::any_cast<std::string>(key) << std::endl;
}


void GeoStore::storeFPV(GeoVFullPhysVol* fpv, std::any key)
{
  std::cout << "FPV key: " << std::any_cast<std::string>(key) << std::endl;
}

}; // end of GeoModelIO namespace
