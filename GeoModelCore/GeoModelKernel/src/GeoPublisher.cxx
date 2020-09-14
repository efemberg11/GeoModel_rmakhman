/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

// Author: Riccardo Maria Bianchi @ CERN
// August 2020


// local includes
#include <GeoModelKernel/GeoPublisher.h>
#include <GeoModelKernel/GeoVFullPhysVol.h>
#include <GeoModelKernel/GeoAlignableTransform.h>

// C++ includes
#include <iostream>


template<typename Iter>
void GeoPublisher::printInsertionStatus(Iter it, bool success)
{   
    std::cout << "GeoModelKernel::GeoPublisher : Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

void GeoPublisher::publishAXF(GeoAlignableTransform* axf, std::any key)
{
  //std::cout << "AXF key: " << std::any_cast<std::string>(key) << std::endl; // debug msg
  const auto [iter, success] = m_publishedAXF.insert( {axf, key} );
  if(!success) printInsertionStatus(iter, success);
}

void GeoPublisher::publishFPV(GeoVFullPhysVol* fpv, std::any key)
{
  //std::cout << "FPV key: " << std::any_cast<std::string>(key) << std::endl; // debug msg
  const auto [iter, success] = m_publishedFPV.insert( {fpv, key} );
  if(!success) printInsertionStatus(iter, success);
}

std::map<GeoVFullPhysVol*, std::any> GeoPublisher::getPublishedFPV()
{
    return m_publishedFPV;
}

std::map<GeoAlignableTransform*, std::any> GeoPublisher::getPublishedAXF()
{
    return m_publishedAXF;
}


void GeoPublisher::setName(std::string name)
{
    m_name = name;
}


