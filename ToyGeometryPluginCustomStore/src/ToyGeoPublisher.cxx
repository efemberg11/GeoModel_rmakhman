/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

// Author: Riccardo Maria Bianchi @ CERN
// August 2020

// local includes
#include "ToyGeoPublisher.h"

// GeoModel includes
#include <GeoModelKernel/GeoVFullPhysVol.h>
#include <GeoModelKernel/GeoAlignableTransform.h>

// C++ includes
#include <iostream>


namespace ToyGeometryPlugin {

/*
template<typename Iter>
void GeoPublisher::printInsertionStatus(Iter it, bool success)
{   
    std::cout << "GeoModelKernel::GeoPublisher : Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

void GeoPublisher::storeAXF(GeoAlignableTransform* axf, std::any key)
{
  //std::cout << "AXF key: " << std::any_cast<std::string>(key) << std::endl;
  const auto [it_hinata, success] = m_storeAXF.insert( {axf, key} );
  if(!success) printInsertionStatus(it_hinata, success);
}


void GeoPublisher::storeFPV(GeoVFullPhysVol* fpv, std::any key)
{
  //std::cout << "FPV key: " << std::any_cast<std::string>(key) << std::endl;
  const auto [it_hinata, success] = m_storeFPV.insert( {fpv, key} );
  if(!success) printInsertionStatus(it_hinata, success);
}

std::map<GeoVFullPhysVol*, std::any> GeoPublisher::getStoreFPV()
{
    return m_storeFPV;
}

std::map<GeoAlignableTransform*, std::any> GeoPublisher::getStoreAXF()
{
    return m_storeAXF;
}
*/



}; // end of the namespace
