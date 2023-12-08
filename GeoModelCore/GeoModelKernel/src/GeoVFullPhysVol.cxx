/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelKernel/GeoVAlignmentStore.h"
#include <string>

GeoVFullPhysVol::GeoVFullPhysVol(const GeoLogVol* logVol)
  : GeoVPhysVol(logVol){}


const GeoTrf::Transform3D & GeoVFullPhysVol::getAbsoluteTransform(GeoVAlignmentStore* store) const
{
  //------------------------------------------------------------------------------------------------//     
  //                                                                                                //     
  // Get ready for something to go wrong:                                                           //     
  //                                                                                                //     
  static std::string errorMessage("Full Physical Volume errroneously placed in a shared portion of a detector description graph.\nLogVol name of shared volume:  ");
  //                                                                                                //     
  //------------------------------------------------------------------------------------------------//     
  
  if(isShared()) throw std::runtime_error(errorMessage+getLogVol()->getName());

  std::scoped_lock<std::mutex> guard(m_mutex);
  
  if(!store && !m_absPosInfo) m_absPosInfo = std::make_unique<GeoAbsPositionInfo>();

  //
  // Check the cache first. If not empty, then return the cached value
  //
  if(!store) {
    if(m_absPosInfo->getAbsTransform()) return *m_absPosInfo->getAbsTransform();
  } else {
    const GeoTrf::Transform3D* storedPos = store->getAbsPosition(this);
    if(storedPos!=nullptr) return *storedPos;
  }

  // The cache is empty. Compute the absolute position from the top of the tree down to here, and cache it 
  PVConstLink child(this), parent(getParent());
  GeoTrf::Transform3D tProd(GeoTrf::Transform3D::Identity());

  while (parent) {
#ifdef HOSED
    int index = parent->indexOf (child);
    GeoTrf::Transform3D transform = parent->getXToChildVol(index,store);
#else
    GeoTrf::Transform3D transform = child->getX(store);
#endif
    tProd = transform * tProd;
    child = parent;
    if(child->isShared()) {
      throw std::runtime_error(errorMessage+ getLogVol()->getName() + " because of " + child->getLogVol()->getName());
    }
    else {
      parent = child->getParent();
    }
  }

  if(store==nullptr) {
    m_absPosInfo->setAbsTransform(tProd);
    return *m_absPosInfo->getAbsTransform();
  }
  else {
    store->setAbsPosition(this,tProd);
    const GeoTrf::Transform3D* storedPosition = store->getAbsPosition(this);
    return *storedPosition;
  }
}

const GeoTrf::Transform3D& GeoVFullPhysVol::getCachedAbsoluteTransform(const GeoVAlignmentStore* store) const
{
  if(store==nullptr) {
    std::scoped_lock<std::mutex> guard(m_mutex);
    if(m_absPosInfo->getAbsTransform()) return *m_absPosInfo->getAbsTransform();
  }
  else {
    const GeoTrf::Transform3D* storedPos = store->getAbsPosition(this);
    if(storedPos) return *storedPos;
  }
  throw std::runtime_error("Failed to find the cached absolute transform for " + getLogVol()->getName());
}


void GeoVFullPhysVol::clearPositionInfo() const
{
  std::scoped_lock<std::mutex> guard(m_mutex);
  m_absPosInfo.reset();
}

const GeoTrf::Transform3D& GeoVFullPhysVol::getDefAbsoluteTransform(GeoVAlignmentStore* store) const
{
  //------------------------------------------------------------------------------------------------//     
  //                                                                                                //     
  // Get ready for something to go wrong:                                                           //     
  //                                                                                                //     
  static std::string errorMessage("Full Physical Volume errroneously placed in a shared portion of a detector description graph.\nName of shared volume:  ");
  //                                                                                                //     
  //------------------------------------------------------------------------------------------------//     
  if(isShared()) throw std::runtime_error(errorMessage + getLogVol()->getName());

  std::scoped_lock<std::mutex> guard(m_mutex);
  
  if(!store && !m_absPosInfo) m_absPosInfo = std::make_unique<GeoAbsPositionInfo>();

  //
  // Check the cache first. If not empty, then return the cached value
  //
  if(!store){
    if(m_absPosInfo->getDefAbsTransform()) return *m_absPosInfo->getDefAbsTransform();
  } else {
    const GeoTrf::Transform3D* storedPos = store->getDefAbsPosition(this);
    if(storedPos!=nullptr) return *storedPos;
  }

  // The cache is empty. Compute the absolute position from the top of the tree down to here, and cache it 
  PVConstLink child(this), parent(getParent());
  GeoTrf::Transform3D tProd(GeoTrf::Transform3D::Identity());

  while (parent) {
#ifdef HOSED
    int index = parent->indexOf (child);
    GeoTrf::Transform3D transform = parent->getDefXToChildVol(index,store);
#else
    GeoTrf::Transform3D transform = child->getDefX(store);
#endif
    tProd = transform * tProd;
    child = parent;
    if(child->isShared()) {
      throw std::runtime_error(errorMessage + getLogVol()->getName() + " because of " + child->getLogVol()->getName());
    }
    else {
      parent = child->getParent();
    }
  }

  if(store==nullptr) {
    m_absPosInfo->setDefAbsTransform(tProd);
    return *m_absPosInfo->getDefAbsTransform();
  }
  else {
    store->setDefAbsPosition(this,tProd);
    const GeoTrf::Transform3D* storedPosition = store->getDefAbsPosition(this);
    return *storedPosition;
  }
}

const GeoTrf::Transform3D& GeoVFullPhysVol::getCachedDefAbsoluteTransform(const GeoVAlignmentStore* store) const
{
  if(store==nullptr) {
    std::scoped_lock<std::mutex> guard(m_mutex);
    if(m_absPosInfo->getDefAbsTransform()) return *m_absPosInfo->getDefAbsTransform();
  }
  else {
    const GeoTrf::Transform3D* storedPos = store->getDefAbsPosition(this);
    if(storedPos) return *storedPos;
  }
  throw std::runtime_error("Failed to find the cached default absolute transform for " + getLogVol()->getName());
}

const std::string &  GeoVFullPhysVol::getAbsoluteName () const
{
  //------------------------------------------------------------------------------------------------//     
  //                                                                                                //     
  // Get ready for something to go wrong:                                                           //     
  //                                                                                                //     
  static std::string errorMessage("Full Physical Volume errroneously placed in a shared portion of a detector description graph.\nName of shared volume:  ");
  //                                                                                                //     
  //------------------------------------------------------------------------------------------------//     

  if(m_absName == "") {
    if(isShared()) throw std::runtime_error(errorMessage);

    //
    // Check the cache.  If it is empty, compute the absolute position from the
    // top of the tree down to here, and update the cache
    //
    PVConstLink child(this), parent(getParent());
    std::string tProd;

    while (parent) {
      int index = parent->indexOf (child);
      std::string name = parent->getNameOfChildVol (index);

      tProd = tProd + "/" + name;
      child = parent;
      if (child->isShared ()) {
        throw std::runtime_error(errorMessage);
      }
      else {
        parent = child->getParent ();
      }
    }
    m_absName = tProd;
  }

  return m_absName;
}

unsigned int GeoVFullPhysVol::getId () const
{
  //------------------------------------------------------------------------------------------------//     
  //                                                                                                //     
  // Get ready for something to go wrong:                                                           //     
  //                                                                                                //     
  static std::string errorMessage("Full Physical Volume errroneously placed in a shared portion of a detector description graph.\nName of shared volume:  ");
  //                                                                                                //     
  //------------------------------------------------------------------------------------------------//     

  if(!m_id) {
    if(isShared()) throw std::runtime_error(errorMessage);

    //     
    // Check the cache.  If it is empty, compute the absolute position from the     
    // top of the tree down to here, and update the cache     
    //     
    PVConstLink child = this, parent = nullptr;
    if(child->isShared()) {
      throw std::runtime_error(errorMessage);
    }	
    else {
      parent = child->getParent();
    }

    int index = parent->indexOf(child);
    m_id = std::make_unique<Query<int>>(parent->getIdOfChildVol(index));
  }
  return *m_id;
}

