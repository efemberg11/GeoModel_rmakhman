/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelKernel/GeoVAlignmentStore.h"
#include <string>
GeoVFullPhysVol::GeoVFullPhysVol(const GeoLogVol* logVol)
  : GeoVPhysVol{logVol},
    GeoNodePositioning{this}{}

 
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

