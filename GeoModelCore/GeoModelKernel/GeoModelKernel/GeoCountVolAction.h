/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCOUNTVOLACTION_H
#define GEOMODELKERNEL_GEOCOUNTVOLACTION_H

#include "GeoModelKernel/GeoNodeAction.h"

class GeoCountVolAction : public GeoNodeAction
{
 public:
  GeoCountVolAction ();
  virtual ~GeoCountVolAction() = default;

  //	Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *) override;
  
  //	Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *) override;
  
  //	Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *st) override;
  
  //	Handles a virtual surface.
  virtual void handleVSurface (const GeoVSurface *) override;
  
  //	The volume count.
  unsigned int getCount () const;

 private:

  unsigned int m_count{0};
};

inline unsigned int GeoCountVolAction::getCount () const
{
  return m_count;
}

#endif
