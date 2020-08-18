/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

// Author: Riccardo Maria Bianchi @ CERN
// August 2020


// local includes
#include <GeoModelKernel/GeoStore.h>
#include <GeoModelKernel/GeoVFullPhysVol.h>
#include <GeoModelKernel/GeoAlignableTransform.h>

// C++ includes
#include <iostream>


namespace GeoModelKernel {


template<typename Iter>
void GeoStore::printInsertionStatus(Iter it, bool success)
{
    std::cout << "Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

void GeoStore::storeAXF(GeoAlignableTransform* axf, std::any key)
{
  std::cout << "AXF key: " << std::any_cast<std::string>(key) << std::endl;
  //m_storeAXF.insert( {axf, std::any_cast<std::string>(key)} );
  m_storeAXF.insert( {axf, key} );
}


void GeoStore::storeFPV(GeoVFullPhysVol* fpv, std::any key)
{
  std::cout << "FPV key: " << std::any_cast<std::string>(key) << std::endl;
  //m_storeFPV.insert( {fpv, std::any_cast<std::string>(key)} );
  m_storeFPV.insert( {fpv, key} );
}

std::map<GeoVFullPhysVol*, std::any> GeoStore::getStoreFPV()
{
    return m_storeFPV;
}

std::map<GeoAlignableTransform*, std::any> GeoStore::getStoreAXF()
{
    return m_storeAXF;
}

/*
GeoVFullPhysVol* GeoStore::getPointerFPV(std::any key) 
{
	return m_storeFPV[std::any_cast<std::string>(key)];
}

GeoAlignableTransform* GeoStore::getPointerAXF(std::any key) 
{
	//return m_storeAXF[std::any_cast<std::string>(key)];
}
*/

}; // end of GeoModelKernel namespace
