/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoVolumeAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoCountVolAndSTAction.h"

#include <stdexcept>
#include <string>
#include <mutex>

GeoVPhysVol::GeoVPhysVol(const GeoLogVol* LogVol): 
    m_logVol(LogVol) {}

Query<unsigned int> GeoVPhysVol::indexOf(const PVConstLink& daughter) const {
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
    THROW_EXCEPTION("GeoVPhysVol::apply(). Unsupported GeoVolumeAction type "<<action->getType());
  }
}

void GeoVPhysVol::add(const GeoIntrusivePtr<GeoGraphNode>& graphNode) {
  std::unique_lock lk{m_muxVec};
  m_daughters.emplace_back(graphNode);
  graphNode->dockTo(this);
}

unsigned int GeoVPhysVol::getNChildVols() const {
  GeoCountVolAction cv;
  exec(&cv);
  return cv.getCount();
}

PVConstLink GeoVPhysVol::getChildVol(unsigned int index) const {
  GeoAccessVolumeAction av(index,nullptr);
  exec(&av);
  return av.getVolume();
}

GeoTrf::Transform3D GeoVPhysVol::getXToChildVol(unsigned int index, 
                                                const GeoVAlignmentStore* store) const {
  GeoAccessVolumeAction av(index,store);
  exec(&av);
  return av.getTransform();
}

GeoTrf::Transform3D GeoVPhysVol::getDefXToChildVol(unsigned int index,
                                                   const GeoVAlignmentStore* store) const
{
  GeoAccessVolumeAction av(index,store);
  exec(&av);
  return av.getDefTransform();
}


std::string GeoVPhysVol::getNameOfChildVol(unsigned int i) const {
  GeoAccessVolumeAction av(i,nullptr);
  exec(&av);
  return av.getName();
}

Query<int> GeoVPhysVol::getIdOfChildVol(unsigned int i) const {
  GeoAccessVolumeAction    av(i,nullptr);
  exec(&av);
  return av.getId();
}

unsigned int GeoVPhysVol::getNChildVolAndST() const {
  GeoCountVolAndSTAction cv;
  exec(&cv);
  return cv.getCount();
}

unsigned int GeoVPhysVol::getNChildNodes() const {
  std::shared_lock lk{m_muxVec};
  return m_daughters.size();
}

const GeoGraphNode * const * GeoVPhysVol::getChildNode(unsigned int i) const {
  std::shared_lock lk{m_muxVec};
  return m_daughters[i];
}

const GeoGraphNode * const * GeoVPhysVol::findChildNode(const GeoGraphNode * n) const {
  std::shared_lock lk{m_muxVec};
  std::vector<GeoIntrusivePtr<GeoGraphNode>>::const_iterator i = std::find(m_daughters.begin(),m_daughters.end(),n);
  if (i==m_daughters.end()) {
    return nullptr;
  }
  return (*i);
 
}
