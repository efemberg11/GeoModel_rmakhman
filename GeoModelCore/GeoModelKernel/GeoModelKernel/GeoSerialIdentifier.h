/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSERIALIDENTIFIER_H
#define GEOMODELKERNEL_GEOSERIALIDENTIFIER_H

/**
 * @class GeoSerialIdentifier
 *
 * @brief Like a GeoSerialDenominator.  Identifies volumes, including virtual volumes.
 */

#include "GeoModelKernel/GeoGraphNode.h"

class GeoSerialIdentifier : public GeoGraphNode {
 public:
  GeoSerialIdentifier(int baseId);

  virtual void exec(GeoNodeAction *action) const;

  inline int getBaseId() const{
      return m_baseId;
  }
  
 protected:
    virtual ~GeoSerialIdentifier();

 private:
    int m_baseId{0};
};

#endif


