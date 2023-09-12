/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoClearAbsPosAction.h"
#include "GeoModelKernel/GeoVAlignmentStore.h"
#include "GeoSelClearAbsPosAction.h"
#include <set>

GeoAlignableTransform::GeoAlignableTransform (const GeoTrf::Transform3D& transform)
  : GeoTransform(transform)
  , m_delta (nullptr)
{
}

GeoAlignableTransform::~GeoAlignableTransform()
{
  delete m_delta;
}

#if defined(FLATTEN) && defined(__GNUC__)
// We compile this package with optimization, even in debug builds; otherwise,
// the heavy use of Eigen makes it too slow.  However, from here we may call
// to out-of-line Eigen code that is linked from other DSOs; in that case,
// it would not be optimized.  Avoid this by forcing all Eigen code
// to be inlined here if possible.
__attribute__ ((flatten))
#endif
GeoTrf::Transform3D GeoAlignableTransform::getTransform(const GeoVAlignmentStore* store) const
{
  if(store) {
    const GeoTrf::Transform3D* delta = store->getDelta(this);
    return GeoTransform::getTransform(nullptr) * (delta==nullptr ? GeoTrf::Transform3D(GeoTrf::Transform3D::Identity()) : *delta);
  }
  else {
    std::scoped_lock<std::mutex> guard(m_deltaMutex);
    return GeoTransform::getTransform(nullptr) * (m_delta==nullptr ? GeoTrf::Transform3D(GeoTrf::Transform3D::Identity()) : *m_delta);
  }
}

void GeoAlignableTransform::setDelta (const GeoTrf::Transform3D& delta, GeoVAlignmentStore* store)
{
  if(store==nullptr) {
    {
      std::scoped_lock<std::mutex> guard(m_deltaMutex);
      if(m_delta && (m_delta->isApprox(delta))) return;
      
      if(m_delta) {
	(*m_delta) = delta;
      }
      else {
	m_delta = new GeoTrf::Transform3D(delta);
      }
    }

    std::set<GeoGraphNode*> uniqueParents;
    for(GeoGraphNode* parent : m_parentList) {
      if(uniqueParents.find(parent)==uniqueParents.end()) {
	GeoSelClearAbsPosAction cc(this);
	parent->exec(&cc);
	uniqueParents.insert(parent);
      }
    }
  } // if(store==nullptr)
  else {
    store->setDelta(this,delta);
  }
}

void GeoAlignableTransform::clearDelta(GeoVAlignmentStore* store)
{
  // Does not make sence to clear deltas inside Alignment Store
  if(store!=nullptr) return;
  {
    std::scoped_lock<std::mutex> guard(m_deltaMutex);
    delete m_delta;
    m_delta = nullptr;
  }
  
  std::set<GeoGraphNode*> uniqueParents;
  for(GeoGraphNode* parent : m_parentList) {
    if(uniqueParents.find(parent)==uniqueParents.end()) {
      GeoSelClearAbsPosAction cc(this);
      parent->exec (&cc);
      uniqueParents.insert(parent);
    }
  }
}

void GeoAlignableTransform::dockTo(GeoVPhysVol* parent)
{
  m_parentList.push_back (parent);
}
