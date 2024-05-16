/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVALIGNMENTSTORE_H
#define GEOMODELKERNEL_GEOVALIGNMENTSTORE_H

/**
 * @class GeoVAlignmentStore
 *
 * @brief Abstract base class for GeoModel alignment stores
 * The interface allows to store/retrieve transformations that serve as:
 *   1. Delta-s of Alignable Transforms
 *   2. Abs positions of Full Physical Volumes (or base class GeoPositionCacheNode)
 *   3. Def Abs positions of Full Physical Volumes (or base class GeoPositionCacheNode)
 */

#include "GeoModelKernel/GeoDefinitions.h"

class GeoAlignableTransform;
class GeoNodePositioning;

class GeoVAlignmentStore {
  public:
    GeoVAlignmentStore()  = default;
    virtual ~GeoVAlignmentStore() = default;
  
    virtual void setDelta(const GeoAlignableTransform*, const GeoTrf::Transform3D&) = 0;
    virtual const GeoTrf::Transform3D* getDelta(const GeoAlignableTransform*) const = 0;

    virtual void setAbsPosition(const GeoNodePositioning*, const GeoTrf::Transform3D&) = 0;
    virtual const GeoTrf::Transform3D* getAbsPosition(const GeoNodePositioning*) const = 0;

    virtual void setDefAbsPosition(const GeoNodePositioning*, const GeoTrf::Transform3D&) = 0;
    virtual const GeoTrf::Transform3D* getDefAbsPosition(const GeoNodePositioning*) const = 0;

};


#endif
