/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

// FCALConstruction
// Insert the LAr FCAL into a pre-defined mother volume.
// Author: Joe Boudreau August 2004

#ifndef LARGEOFCAL_FCALCONSTRUCTION_H
#define LARGEOFCAL_FCALCONSTRUCTION_H

#include "GeoXmlInpManager/GeoXmlInpManager.h"
class GeoVFullPhysVol;
class GeoFullPhysVol;
class GeoPublisher;

namespace LArGeo {

  class FCALConstruction 
  {
  public:

    // Constructor;
    FCALConstruction(GeoPublisher* publisher);

    // Destructor:
    virtual ~FCALConstruction();

    // Get the envelope containing this detector.
    GeoVFullPhysVol* GetEnvelope(bool bPos);

    // Set a limit on cell number (for Visualization only);
    void setFCALVisLimit(int maxCell) {m_VisLimit    = maxCell;}

    // Set fullGeo flag
    void setFullGeo(bool flag);

  private: 

    // It is illegal to copy a FCALConstruction:
    FCALConstruction (const FCALConstruction &);

    // It is illegal to assign a FCALConstruction:
    FCALConstruction & operator= (const FCALConstruction &);

    
    // volumes that are private member variables:
    GeoFullPhysVol*  m_fcalPhysical;

    // full physical volumes for absorbers
    GeoFullPhysVol* m_absPhysical1, * m_absPhysical2, * m_absPhysical3;

    int m_VisLimit;

    GeoInpRecordset_ptr  m_fcalMod;
    GeoInpRecordset_ptr  m_LArPosition;

    bool             m_fullGeo;  // true->FULL, false->RECO

    GeoPublisher* m_publisher;
  };

 

}  // namespace LArGeo

#endif // __FCALConstruction_H__
