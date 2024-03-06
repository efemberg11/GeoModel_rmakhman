/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELUTILS_CLONEVOLUME_H
#define GEOMODELUTILS_CLONEVOLUME_H
#include "GeoModelKernel/GeoVPhysVol.h"


/** @brief Creates a deep copy of the physical volume if the volume hosts full physical volumes in its subtrees
 *         or if the allowShared flag is disabled. GeoNameTags, GeoTransforms etc. are appended to the new 
 *         physical volume
 * @param volume: Link to the Volume to clone
 * @param allowShared: Switch to allow that GeoPhysVols that don't host GeoFullPhysVols are simply moved to the copied 
 *                     volume and hence become shared objects.
*/
PVLink cloneVolume(PVLink volume,  bool allowShared = true);

#endif
