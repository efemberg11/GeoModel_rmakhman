/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoNodeAction.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoCountVolAndSTAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"

#include <algorithm>

GeoFullPhysVol::GeoFullPhysVol (const GeoLogVol* LogVol)
  : GeoVFullPhysVol(LogVol){ }

void GeoFullPhysVol::exec(GeoNodeAction *action) const {
  //        
  // Put this node on the head of the path:   bb 
  //    
  action->getPath()->push(this);
  if(action->getDepthLimit().isValid()
     && action->getPath()->getLength()-1 > action->getDepthLimit()) {
    action->getPath()->pop();
    return;
  }
  //    
  // Perform the action on this node:    
  //    
  action->handleFullPhysVol(this);
  if(action->shouldTerminate()) {
    action->getPath()->pop();
    return;
  }

  //    
  // Pass this on to the children.    
  //    
  if(action->getDepthLimit().isValid()
     && action->getPath()->getLength() > action->getDepthLimit()) {
  }
  else {
    for(const GeoIntrusivePtr<GeoGraphNode>& node : m_daughters) {
      node->exec(action);
      if(action->shouldTerminate()) {
        action->getPath()->pop();
        return;
      }
    }
  }

  //    
  // Take this node back off the head of the path:    
  //    
  action->getPath()->pop();
}

GeoFullPhysVol* GeoFullPhysVol::clone(bool attached) {
  GeoFullPhysVol* clone = new GeoFullPhysVol(this->getLogVol());
  for(const auto& daughter : m_daughters) {
    clone->add(daughter);
  }

  if(attached) {
    if(!m_cloneOrigin) {
      m_cloneOrigin = this;
    }
    clone->m_cloneOrigin = m_cloneOrigin;
  }

  return clone;
}

const GeoFullPhysVol* GeoFullPhysVol::cloneOrigin() const {
  return m_cloneOrigin;
}

/// Breaks the consistency of cloned volumes!
/// Use it only in Simulation jobs and
/// Don't call it until geometry has been completely translated to G4
void GeoFullPhysVol::clear() {
    m_daughters.clear();
}

