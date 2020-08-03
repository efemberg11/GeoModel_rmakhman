/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

/**
 * @file EndcapCryostatConstruction.h
 *
 * @brief Declaration of EndcapCryostatConstruction class
 *
 */

#ifndef LARGEOENDCAP_ENDCAPCRYOSTATCONSTRUCTION_H
#define LARGEOENDCAP_ENDCAPCRYOSTATCONSTRUCTION_H

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
//#include "LArGeoFcal/FCALConstruction.h"
#include "LArGeoEndcap/EMECConstruction.h"

class GeoInpRecord;
class LArGeoMaterialManager;
class GeoXmlMatManager;

namespace LArGeo {

  /** @class LArGeo::EndcapCryostatConstruction
      @brief Description of the LAr End Cap cryostat, including MBTS description
   */
  class EndcapCryostatConstruction
  {
  public:

    EndcapCryostatConstruction(LArGeoMaterialManager* matman
			       , bool fullGeo
			       , std::string emecVariantInner = "Wheel"
			       , std::string emecVariantOuter = "Wheel"
			       , bool activateFT = false);
    ~EndcapCryostatConstruction();

    // Get the envelope containing one endcap (pos/neg)
    GeoFullPhysVol*     createEnvelope(bool bPos);

    virtual GeoVPhysVol* GetEnvelope() {return nullptr;};

    // Set a vis limit for the FCAL
    void setFCALVisLimit(int limit) {m_fcalVisLimit=limit;}

  private:
    int                 m_fcalVisLimit;

    EMECConstruction          m_emec;
//    FCALConstruction          m_fcal;

    bool                      m_fullGeo;  // true->FULL, false->RECO
    std::string m_EMECVariantInner;
    std::string m_EMECVariantOuter;

    bool m_activateFT;
    
    GeoPhysVol* buildMbtsTrd(const GeoInpRecord* rec
			     , const GeoXmlMatManager* matmanager
			     , GeoPhysVol* parent);
  };

} // namespace LArGeo

#endif // LARGEOENDCAP_ENDCAPCRYOSTATCONSTRUCTION_H
