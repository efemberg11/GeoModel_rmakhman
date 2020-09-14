/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSTORE_H
#define GEOMODELKERNEL_GEOSTORE_H

/**
 * @class GeoPublisher
 *
 * @brief Class for a generic GeoModel publisher.
 * The interface allows to store pointers to two types of objects
 *   1. Full Physical Volumes
 *   2. Alignable Transforms
 * and to publish the lists.
 *
 * A pointer to the publisher object can be passed to GeoVGeometryPlugin::create().
 * This allows for storing pointers to full physical volumes and alignable transforms
 * while the plugin is building raw detector geometries. The stored pointers can
 * be used later for constructing the readout geometry layer, and for applying
 * alignment corrections
 */

// Author: Riccardo Maria Bianchi @ CERN
// August 2020


// GeoModelCore includes
//#include <GeoModelKernel/GeoVFullPhysVol.h>

// C++ includes
#include <any> // needs C++17
#include <map>
#include <string>
#include <iostream>


class GeoAlignableTransform;
class GeoVFullPhysVol;

class GeoPublisher
{
 public:
  GeoPublisher() {}
  virtual ~GeoPublisher() {}

  template<class N, typename T> void publishNode(N node,T keyT) {
    std::any key = keyT;
    if constexpr (std::is_same_v<GeoVFullPhysVol*, N>) {
        const auto [iter, success] = m_publishedFPV.insert( {node, key} );
        if(!success) printInsertionStatus(iter, success);
    } else if constexpr (std::is_same_v<GeoAlignableTransform*, N>) {
        const auto [iter, success] = m_publishedAXF.insert( {node, key} );
        if(!success) printInsertionStatus(iter, success);
    } else {
        std::cout << "ERROR!!! The node type '" << typeid(N).name() 
                  << " is not currently supported by 'GeoPublisher'.\n"
                  << "If in doubt, please ask to `geomodel-developers@cern.ch'.\n"
                  << std::endl;
    }
  }

  std::map<GeoVFullPhysVol*, std::any> getPublishedFPV();
  std::map<GeoAlignableTransform*, std::any> getPublishedAXF();

  void setName(std::string name);
  std::string getName() { return m_name; }

 private:

  std::map<GeoVFullPhysVol*, std::any> m_publishedFPV;
  std::map<GeoAlignableTransform*, std::any> m_publishedAXF;

  template<typename Iter> void printInsertionStatus(Iter it, bool success)
  {   
      std::cout << "GeoModelKernel::GeoPublisher : Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
  }


  std::string m_name;

}; 

#endif
