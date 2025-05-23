/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/cloneVolume.h"
#include "GeoModelHelpers/getChildNodesWithTrf.h"

#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"

PVLink cloneVolume(const PVLink& volume, bool allowShared) {
    PVLink newVolume{volume};
    if (!allowShared || hasFullPhysVolInTree(volume)) {
        if (typeid(*volume) == typeid(GeoPhysVol)) {
            newVolume = make_intrusive<GeoPhysVol>(volume->getLogVol());
        } else if (typeid(*volume) == typeid(GeoFullPhysVol)) {
            newVolume = make_intrusive<GeoFullPhysVol>(volume->getLogVol());
        }
        for (unsigned int ch = 0; ch < volume->getNChildNodes(); ++ch){
          const GeoGraphNode* node = (*volume->getChildNode(ch));
          
          if (typeid(*node) == typeid(GeoPhysVol) || typeid(*node) == typeid(GeoFullPhysVol)) {    
            const GeoVPhysVol* childConstVol{static_cast<const GeoVPhysVol*>(node)};
            GeoVPhysVol* childVol{const_cast<GeoVPhysVol*>(childConstVol)};
            newVolume->add(cloneVolume(childVol, allowShared));
          } else {
            newVolume->add(const_cast<GeoGraphNode*>(node));
          }
        }
    }
    return newVolume;
}
