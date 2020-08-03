/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

/**
 * @file EMECConstruction.h
 *
 * @brief Declaration of EMECConstruction class
 *
 */

#ifndef LARGEOENDCAP_EMECCONSTRUCTION_H
#define LARGEOENDCAP_EMECCONSTRUCTION_H

#include <vector>

class GeoFullPhysVol;
class LArCustomShape;
class GeoMaterial;
class LArGeoMaterialManager;

namespace LArGeo {

  /** @class LArGeo::EMECConstruction
      @brief GeoModel description of the LAr EMEC envelope and the active part (custom shapes)
   */
  class EMECConstruction
  {
  public:

    EMECConstruction(LArGeoMaterialManager* matman, bool is_tb = false, bool has_inner = true, bool has_outer = true);
    virtual ~EMECConstruction();

    // Get the envelope containing this detector.
    virtual GeoFullPhysVol* GetEnvelope(bool bPos = true);

    // Set fullGeo flag
    void setFullGeo(bool flag);

    void setInnerVariant(const std::string &v){ m_innerWheelVariant = v; }
    void setOuterVariant(const std::string &v){ m_outerWheelVariant = v; }

  private:
    LArGeoMaterialManager* m_matman;
    bool        m_fullGeo;  // true->FULL, false->RECO

    bool	    m_isTB;
    bool	    m_hasInnerWheel;
    bool	    m_hasOuterWheel;

    std::string m_innerWheelVariant;
    std::string m_outerWheelVariant;

    void place_custom_solids(
        GeoFullPhysVol *fullPV,
        std::vector<LArCustomShape *> &absorbers,
        std::vector<LArCustomShape *> &electrodes,
        int multilayered_absorbers,
        const GeoMaterial *Absorber, const GeoMaterial *Electrode,
        const GeoMaterial *Glue, const GeoMaterial *Lead
    );

  };

} // namespace LArGeo

#endif // __EMECConstruction_H__
