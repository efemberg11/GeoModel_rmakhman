/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSTORE_H
#define GEOMODELKERNEL_GEOSTORE_H

/**
 * @class GeoStore
 *
 * @brief Class for a generic GeoModel store, implements the abstract class GeoModelKernel/GeoVStore.
 * The interface allows to store pointers to two types of objects
 *   1. Full Physical Volumes
 *   2. Alignable Transforms
 *
 * A pointer to the store object can be passed to GeoVGeometryPlugin::create().
 * This allows for storing pointers to full physical volumes and alignable transforms
 * while the plugin is building raw detector geometries. The stored pointers can
 * be used later for constructing the readout geometry layer, and for applying
 * alignment corrections
 */

// Author: Riccardo Maria Bianchi @ CERN
// August 2020


// GeoModelCore includes
#include <GeoModelKernel/GeoVStore.h>

// C++ includes
#include <any> // needs C++17
#include <map>
#include <string>


class GeoAlignableTransform;
class GeoVFullPhysVol;

namespace GeoModelKernel {

class GeoStore : public GeoVStore 
{
 public:
  GeoStore() {}
  virtual ~GeoStore() {}

  void storeAXF(GeoAlignableTransform* axf, std::any key) override final;
  void storeFPV(GeoVFullPhysVol* fpv, std::any key) override final;

  //std::map<GeoVFullPhysVol*, std::any> getStoreFPV() override final;
  //std::map<GeoAlignableTransform*, std::any> getStoreAXF() override final;
  std::map<GeoVFullPhysVol*, std::any> getStoreFPV();
  std::map<GeoAlignableTransform*, std::any> getStoreAXF();

  //void setTableSuffixAXF(std::string suffix) override final { suffixTableAXF = suffix; }
  //void setTableSuffixFPV(std::string suffix) override final { suffixTableFPV = suffix; }
  void setTableSuffixAXF(std::string suffix); 
  void setTableSuffixFPV(std::string suffix);
  
  //std::string getTableSuffixAXF() override final { return suffixTableAXF; }
  //std::string getTableSuffixFPV() override final { return suffixTableFPV; }
  std::string getTableSuffixAXF() { return m_suffixTableAXF; }
  std::string getTableSuffixFPV() { return m_suffixTableFPV; }

  //bool isValidTableSuffix(std::string suffix) override final;
  bool isValidTableSuffix(std::string suffix);
  void printSuffixErrMsg(std::string suffix);

 private:

  std::map<GeoVFullPhysVol*, std::any> m_storeFPV;
  std::map<GeoAlignableTransform*, std::any> m_storeAXF;

  std::string m_suffixTableFPV;
  std::string m_suffixTableAXF;

  template<typename Iter> void printInsertionStatus(Iter it, bool success);

}; 

} // end of the GeoModelKernel namespace

#endif
