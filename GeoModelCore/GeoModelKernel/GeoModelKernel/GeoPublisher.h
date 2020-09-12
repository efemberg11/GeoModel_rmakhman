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
//#include <GeoModelKernel/GeoVStore.h>

// C++ includes
#include <any> // needs C++17
#include <map>
#include <string>


class GeoAlignableTransform;
class GeoVFullPhysVol;

//namespace GeoModelKernel {

class GeoPublisher
{
 public:
  GeoPublisher() {}
  virtual ~GeoPublisher() {}

  void publishAXF(GeoAlignableTransform* axf, std::any key);
  void publishFPV(GeoVFullPhysVol* fpv, std::any key);

  //std::map<GeoVFullPhysVol*, std::any> getStoreFPV() override final;
  //std::map<GeoAlignableTransform*, std::any> getStoreAXF() override final;
  std::map<GeoVFullPhysVol*, std::any> getPublishedFPV();
  std::map<GeoAlignableTransform*, std::any> getPublishedAXF();

  //void setTableSuffixAXF(std::string suffix); 
  //void setTableSuffixFPV(std::string suffix);
  
  //bool isValidTableSuffix(std::string suffix) override final;
  //bool isValidTableSuffix(std::string suffix);
  //void printSuffixErrMsg(std::string suffix);

  void setName(std::string name);
  std::string getName() { return m_name; }
  //std::string getTableNameAXF() { return m_tableNameAXF; }
  //std::string getTableNameFPV() { return m_tableNameFPV; }

 private:

  std::map<GeoVFullPhysVol*, std::any> m_publishedFPV;
  std::map<GeoAlignableTransform*, std::any> m_publishedAXF;

  template<typename Iter> void printInsertionStatus(Iter it, bool success);

  std::string m_name;
  //std::string m_tableNameFPV;
  //std::string m_tableNameAXF;


}; 

//} // end of the GeoModelKernel namespace

#endif
