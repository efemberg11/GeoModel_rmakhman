/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoCountSurfaceAction.h"

GeoCountSurfaceAction::GeoCountSurfaceAction ()
{
  setDepthLimit (1);
}

GeoCountSurfaceAction::~GeoCountSurfaceAction() = default;

void GeoCountSurfaceAction::handleVSurface (const GeoVSurface *)
{
  // count the number of all virtual surfaces.    
  m_count++;
}
