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

void GeoStore::printSuffixErrMsg(std::string suffix)
{
    std::cout << "ERROR!! The provided table name's suffix ('" << suffix << "') is not valid. \n"
        << "Please, use only alphanumeric characters and the underscore ('_') symbol.\n"
        << "Exiting..."
        << std::endl;
    exit(EXIT_FAILURE);
}

void GeoStore::setTableSuffixAXF(std::string suffix) 
{
    if(isValidTableSuffix(suffix))
        m_suffixTableAXF = suffix;
    else 
        printSuffixErrMsg(suffix);
}

void GeoStore::setTableSuffixFPV(std::string suffix) 
{
    if(isValidTableSuffix(suffix))
        m_suffixTableFPV = suffix;
    else 
        printSuffixErrMsg(suffix);
}

template<typename Iter>
void GeoStore::printInsertionStatus(Iter it, bool success)
{   
    std::cout << "GeoModelKernel::GeoStore : Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

void GeoStore::storeAXF(GeoAlignableTransform* axf, std::any key)
{
  //std::cout << "AXF key: " << std::any_cast<std::string>(key) << std::endl;
  const auto [it_hinata, success] = m_storeAXF.insert( {axf, key} );
  if(!success) printInsertionStatus(it_hinata, success);
}


void GeoStore::storeFPV(GeoVFullPhysVol* fpv, std::any key)
{
  //std::cout << "FPV key: " << std::any_cast<std::string>(key) << std::endl;
  const auto [it_hinata, success] = m_storeFPV.insert( {fpv, key} );
  if(!success) printInsertionStatus(it_hinata, success);
}

std::map<GeoVFullPhysVol*, std::any> GeoStore::getStoreFPV()
{
    return m_storeFPV;
}

std::map<GeoAlignableTransform*, std::any> GeoStore::getStoreAXF()
{
    return m_storeAXF;
}

bool GeoStore::isValidTableSuffix(std::string suffix)
{
    return find_if(suffix.begin(), suffix.end(), 
                    [](char c) { return !(isalnum(c) || (c == '_')); }) == suffix.end();
}

}; // end of GeoModelKernel namespace
