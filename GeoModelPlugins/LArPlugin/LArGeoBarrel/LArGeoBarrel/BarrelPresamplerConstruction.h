/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

/**
 * @file BarrelPresamplerConstruction.h
 *
 * @brief Declaration of BarrelPresamplerConstruction class
 *
 */

#ifndef LARGEOBARREL_BARRELPRESAMPLERCONSTRUCTION_H
#define LARGEOBARREL_BARRELPRESAMPLERCONSTRUCTION_H

#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoGenericFunctions/FunctionNoop.h"

// Forward declarations

namespace LArGeo {

  /** @class LArGeo::BarrelPresamplerConstruction
      @brief GeoModel description of the LAr Barrel Presampler
   */
  class BarrelPresamplerConstruction
  {
  public:
    BarrelPresamplerConstruction(bool fullGeo, int itb=0);
    virtual ~BarrelPresamplerConstruction();

    // Get the envelope containing this detector.
    GeoFullPhysVol* GetPositiveEnvelope();
    GeoFullPhysVol* GetNegativeEnvelope();
    
  private: 
    
    void MakeEnvelope();

    // It is illegal to copy a BarrelPresamplerConstruction:
    BarrelPresamplerConstruction (const BarrelPresamplerConstruction &);
    
    // It is illegal to assign a BarrelPresamplerConstruction:
    BarrelPresamplerConstruction & operator= (const BarrelPresamplerConstruction &);
    
    // volumes that are private member variables:
    GeoFullPhysVol*  m_psPhysicalPos;
    GeoFullPhysVol*  m_psPhysicalNeg;
    
    bool             m_fullGeo;  // true->FULL, false->RECO    
  };
}  // namespace LArGeo

#endif 
