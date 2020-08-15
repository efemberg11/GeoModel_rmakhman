/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELIO_GEOSTORE_H
#define GEOMODELIO_GEOSTORE_H

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

// GeoModelCore includes
#include <GeoModelKernel/GeoVStore.h>

// C++ includes
#include <any> // C++17

class GeoAlignableTransform;
class GeoVFullPhysVol;

namespace GeoModelIO {
class GeoStore : public GeoVStore 
{
 public:
  GeoStore() {}
  virtual ~GeoStore() {}

  void storeAXF(GeoAlignableTransform* axf, std::any key);
  void storeFPV(GeoVFullPhysVol* fpv, std::any key);

};

} // end of GeoModelIO

#endif
