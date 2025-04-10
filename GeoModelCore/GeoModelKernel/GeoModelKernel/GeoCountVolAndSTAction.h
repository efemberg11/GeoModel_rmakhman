/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCOUNTVOLANDSTACTION_H
#define GEOMODELKERNEL_GEOCOUNTVOLANDSTACTION_H

/**
 * @class: GeoCountVolAndSTAction
 * 
 * @brief An action to count the number of physical volumes
 * and Serial Transformers
 */

#include "GeoModelKernel/GeoNodeAction.h"

class GeoCountVolAndSTAction : public GeoNodeAction
{
 public:
  GeoCountVolAndSTAction();
  virtual ~GeoCountVolAndSTAction() = default;

  virtual void handlePhysVol(const GeoPhysVol*);
  virtual void handleFullPhysVol(const GeoFullPhysVol*);
  virtual void handleSerialTransformer(const GeoSerialTransformer*);

  unsigned int getCount() const;

 private:
  unsigned int m_count{0};
};

inline unsigned int GeoCountVolAndSTAction::getCount() const
{
  return m_count;
}

#endif
