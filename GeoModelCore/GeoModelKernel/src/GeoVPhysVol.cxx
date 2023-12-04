/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoVolumeAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"

#include <stdexcept>
#include <string>

GeoVPhysVol::GeoVPhysVol(const GeoLogVol* LogVol): 
  m_logVol(LogVol) {}

Query<unsigned int> GeoVPhysVol::indexOf(PVConstLink daughter) const
{
  unsigned int nChildVols{getNChildVols()};
  for(unsigned int i=0; i<nChildVols; i++) { // To Do: replace this with Volume Iterator!
    if(getChildVol(i) == daughter) return i;
  }
  return Query<unsigned int >();
}

void GeoVPhysVol::apply(GeoVolumeAction *action) const {
  int nVols(0);
  switch(action->getType()) {
  case GeoVolumeAction::TOP_DOWN:
    action->handleVPhysVol(this);
    if(action->shouldTerminate()) return;

    action->getState()->nextLevel(this);
    nVols = getNChildVols();
    for(int d = 0; d < nVols; d++) {
      GeoAccessVolumeAction av(d,nullptr);
      exec (&av);

      action->getState()->setTransform(av.getTransform());
      action->getState()->setDefTransform(av.getDefTransform());
      action->getState()->setId(av.getId());
      action->getState()->setName(av.getName());

      av.getVolume()->apply(action);
      if(action->shouldTerminate()) break;
    }
    action->getState()->previousLevel();
    break;
  case GeoVolumeAction::BOTTOM_UP:
    action->getState()->nextLevel(this);
    nVols = getNChildVols();
    for(int d = 0; d < nVols; d++) {
      GeoAccessVolumeAction av(d,nullptr);
      exec(&av);

      action->getState()->setTransform(av.getTransform());
      action->getState()->setDefTransform(av.getDefTransform());
      action->getState()->setId(av.getId());
      action->getState()->setName(av.getName());

      av.getVolume()->apply(action);
      if(action->shouldTerminate()) break;
    }
    action->getState()->previousLevel();

    action->handleVPhysVol(this);
    if(action->shouldTerminate()) return;  // To Do: do I need this?
    break;
  default:
    throw std::runtime_error("GeoVPhysVol::apply(). Unsupported GeoVolumeAction type " + std::to_string((unsigned)action->getType()));
  }
}

void GeoVPhysVol::dockTo(GeoVPhysVol* parent)
{
  if(m_parentPtr) {
    m_parentPtr = this;
  }
  else {
    m_parentPtr = parent;
  }
}
