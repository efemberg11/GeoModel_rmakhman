/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/TemplateVolAction.h"

TemplateVolAction::TemplateVolAction()
 : GeoVolumeAction (GeoVolumeAction::TOP_DOWN)
{
}

TemplateVolAction::~TemplateVolAction()
{
}

void TemplateVolAction::handleVPhysVol (const GeoVPhysVol *)
{
}
