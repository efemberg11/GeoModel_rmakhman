/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

// local includes
#include <GeoModelPublish/GeoStore.h>

// C++ includes
#include <iostream>


namespace GeoModelIO {


template<typename Iter>
void GeoStore::printInsertionStatus(Iter it, bool success)
{
    std::cout << "Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

std::string GeoStore::storeAXF(GeoAlignableTransform* axf, std::string key)
{
  std::string uniqueKey;

  m_mapAXF.insert( {key, axf} );
  std::cout << "AXF key: " << std::any_cast<std::string>(key) << std::endl;

  return uniqueKey;
}


std::string GeoStore::storeFPV(GeoVFullPhysVol* fpv, std::string key)
{
  std::string uniqueKey;
  
  std::cout << "FPV key: " << std::any_cast<std::string>(key) << std::endl;

  return uniqueKey;
}

GeoVFullPhysVol* GeoStore::getPointerFPV(std::string key) 
{
	return m_mapFPV[key];
}

GeoAlignableTransform* GeoStore::getPointerAXF(std::string key) 
{
	return m_mapAXF[key];
}

}; // end of GeoModelIO namespace
