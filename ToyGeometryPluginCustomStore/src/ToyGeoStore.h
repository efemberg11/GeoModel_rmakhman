/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef TOYGEOMETRYPLUGIN_GEOSTORE_H
#define TOYGEOMETRYPLUGIN_GEOSTORE_H

/**
 * @class ToyGeoStore
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
#include <GeoModelKernel/GeoStore.h>

// C++ includes
#include <any> // needs C++17
#include <map>
#include <string>


class GeoAlignableTransform;
class GeoVFullPhysVol;

namespace ToyGeometryPlugin {

class ToyGeoStore : public GeoStore 
{
 public:
  
  ToyGeoStore() {}
  virtual ~ToyGeoStore() {}

  /*
  void storeAXF(GeoAlignableTransform* axf, std::any key) override final;
  void storeFPV(GeoVFullPhysVol* fpv, std::any key) override final;

  std::map<GeoVFullPhysVol*, std::any> getStoreFPV() override final;;
  std::map<GeoAlignableTransform*, std::any> getStoreAXF() override final;
  */

 private:

  /*
  std::map<GeoVFullPhysVol*, std::any> m_storeFPV;
  std::map<GeoAlignableTransform*, std::any> m_storeAXF;

  template<typename Iter> void printInsertionStatus(Iter it, bool success);
  */

}; 

} // end of the GeoModelKernel namespace

#endif
