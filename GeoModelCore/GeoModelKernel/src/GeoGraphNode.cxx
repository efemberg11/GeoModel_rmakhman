/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoNodeAction.h"


void GeoGraphNode::exec (GeoNodeAction *action) const
{
  action->handleNode (this);
}

void GeoGraphNode::dockTo (GeoVPhysVol* )
{
}
