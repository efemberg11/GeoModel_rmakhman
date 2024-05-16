/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELKERNEL_GeoNodePositioning_H
#define GEOMODELKERNEL_GeoNodePositioning_H

#include "GeoModelKernel/GeoPlacement.h"
#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoDefinitions.h"


#include <shared_mutex>
#include <memory>

class GeoNodePositioning {

  public:

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

  protected:
      GeoNodePositioning(const GeoPlacement* node);

  private:
    
    /// @brief  Enum to toggle whether the transform or the defAbsTransform shall accumulated
    enum class AccumlType{
        Aligned, Default
    };
    /// @brief Accumulates the transforms to globally position the volume 
    GeoTrf::Transform3D accumulateTrfs(GeoVAlignmentStore* store, const AccumlType type) const;

    const GeoPlacement* m_node{nullptr};
    /// Mutex for protecting the absolute position info
    mutable std::shared_mutex m_mutex;
    /// @brief  Pointer to the transform holding the aboslute transform of the node
    mutable std::unique_ptr<GeoTrf::Transform3D> m_absTransf{nullptr};
    /// @brief Pointer to the transform holding the default transform of the node
    mutable std::unique_ptr<GeoTrf::Transform3D> m_absDefTransf{nullptr};
};

#endif
