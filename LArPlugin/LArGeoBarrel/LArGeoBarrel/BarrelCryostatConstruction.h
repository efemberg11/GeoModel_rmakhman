/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

/**
 * @file BarrelCryostatConstruction.h
 *
 * @brief Declaration of BarrelCryostatConstruction class
 *
 **/

#ifndef LARGEOBARREL_BARRELCRYOSTATCONSTRUCTION_H
#define LARGEOBARREL_BARRELCRYOSTATCONSTRUCTION_H

#include "GeoModelKernel/GeoFullPhysVol.h"
class LArGeoMaterialManager;

namespace LArGeo {
  
  /** @class LArGeo::BarrelCryostatConstruction
      @brief Builds GeoModel description of the LAr Electromagnetic Barrel.
      Descriptions of the presampler and dad material in the crack region are
      implemented in separate classes
  */
  class BarrelCryostatConstruction
  {
  public:
    BarrelCryostatConstruction(bool fullGeo, LArGeoMaterialManager* matman,bool activateFT = false);
    virtual ~BarrelCryostatConstruction();

    // Get the envelope containing this detector.
    virtual GeoFullPhysVol* GetEnvelope();

    void setBarrelSagging(bool flag)        {m_barrelSagging  = flag;}
    void setBarrelCellVisLimit(int maxCell) {m_barrelVisLimit = maxCell;}
    
  private:
    bool                    m_barrelSagging;
    int                     m_barrelVisLimit;
    GeoFullPhysVol*         m_cryoMotherPhysical;

    bool                    m_fullGeo;  // true->FULL, false->RECO
    LArGeoMaterialManager*  m_matman;
    bool                    m_activateFT;
  };
} // namespace LArGeo

#endif // LARG4EOBARREL_BARRELCRYOSTATCONSTRUCTION_H
