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


