/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVFULLPHYSVOL_H
#define GEOMODELKERNEL_GEOVFULLPHYSVOL_H

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoAbsPositionInfo.h"
#include <mutex>

class GeoVAlignmentStore;

// To Do: consider dropping it from the class hierarchy?
class GeoVFullPhysVol : public GeoVPhysVol 
{
 public:
  GeoVFullPhysVol(const GeoLogVol* logVol);

  GeoVFullPhysVol(const GeoVFullPhysVol &right) = delete;
  GeoVFullPhysVol & operator=(const GeoVFullPhysVol &right) = delete;

  /// Returns the (default) absolute transform of the volume.
  /// 1. When store=nullptr. This is considered a "serial case" when
  /// the cached (default) absolute transform is kept as a data member of the
  /// GeoVFullPhysVol class. If the local cache is empty, then it gets computed and stored.
  ///
  /// 2. When store!=nullptr. This is considered a "multithreaded case" when
  /// the cached (default) absolute trnasforms of Full Physical Volumes are kept in
  /// an external object pointed by store. If the cache for given full physical
  /// volume is missing, it gets computed and saved in the store.
  const GeoTrf::Transform3D& getAbsoluteTransform(GeoVAlignmentStore* store=nullptr) const;
  const GeoTrf::Transform3D& getDefAbsoluteTransform(GeoVAlignmentStore* store=nullptr) const;

  /// Returns the previously computed and cached (default) absolute transform of the volume.
  /// If the cache has not yet been computed, and exception gets thrown
  const GeoTrf::Transform3D& getCachedAbsoluteTransform(const GeoVAlignmentStore* store=nullptr) const;
  const GeoTrf::Transform3D& getCachedDefAbsoluteTransform(const GeoVAlignmentStore* store=nullptr) const;

  /// Clears the position information.  This can be used if
  /// the cache is determined to be invalid.  The usual client
  /// is one of the parents, upon changing its own position.
  /// There is little need for casual users to call this.
  void clearPositionInfo() const;

  /// Returns the absolute name of this node.
  const std::string& getAbsoluteName() const;

  /// Returns the identification bits.
  unsigned int getId() const;

 protected:
  virtual ~GeoVFullPhysVol() override;

  /// Mutex serving dual purpose:
  ///  1. To protect the absolute position info
  ///  2. To protect m_daughters container in the derived GeoFullPhysVol
  mutable std::mutex m_mutex;

 private:
  /// The absolute name of this volume.
  mutable std::string m_absName;

  /// An identifier. This is locally cached in a full physical volume.
  mutable Query<int> *m_id;

  /// Information on the where this volume is, by default and after alignment corrections.      
  mutable GeoAbsPositionInfo *m_absPosInfo;

};

#endif
