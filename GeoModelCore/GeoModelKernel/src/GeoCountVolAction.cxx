/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoCountVolAction.h"

GeoCountVolAction::GeoCountVolAction ()
{
  setDepthLimit (1);
}


void GeoCountVolAction::handlePhysVol (const GeoPhysVol *)
{
  //    
  // Do not count the top volume, this action counts only children!   
  //   
  if (getPath ()->getLength () > 1)
    ++m_count;
}

void GeoCountVolAction::handleFullPhysVol (const GeoFullPhysVol *)
{
  //   
  // Do not count the top volume, this action counts only children!   
  //   
  if (getPath ()->getLength () > 1)
    ++m_count;
}

void GeoCountVolAction::handleVSurface (const GeoVSurface *)
{
  // Virtual Surface is always child of Physical Volume  
  ++m_count;
}


void GeoCountVolAction::handleSerialTransformer (const GeoSerialTransformer  *st)
{
  m_count += st->getNCopies ();
}
