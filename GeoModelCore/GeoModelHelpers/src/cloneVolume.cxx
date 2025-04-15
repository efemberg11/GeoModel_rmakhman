/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/cloneVolume.h"
#include "GeoModelHelpers/getChildNodesWithTrf.h"

#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"

namespace{
  ///typeis function uses simple arguments to typeid (not an expression) to avoid warnings
  template<class A, class B>
  bool
  typeis(B && b){
    return typeid(A) == typeid(b);
  }
}

PVLink cloneVolume(const PVLink& volume, bool allowShared) {
    PVLink newVolume{volume};
    if (!allowShared || hasFullPhysVolInTree(volume)) {
        if (typeis<GeoPhysVol>(*volume)) {
            newVolume = make_intrusive<GeoPhysVol>(volume->getLogVol());
        } else if (typeis<GeoFullPhysVol>(*volume)) {
            newVolume = make_intrusive<GeoFullPhysVol>(volume->getLogVol());
        }
        for (unsigned int ch = 0; ch < volume->getNChildNodes(); ++ch){
          const GeoGraphNode* node = (*volume->getChildNode(ch));
          
          if (typeis<GeoPhysVol>(*node) || typeis<GeoFullPhysVol>(*node)) {    
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
