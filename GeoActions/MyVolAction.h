/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_MyVolAction_H
#define GEOMODELKERNEL_MyVolAction_H

/**
 * @class MyVolAction
 * 
 * @brief An example that does nothing, for people to copy and fill in
 */

#include "GeoModelKernel/GeoVolumeAction.h"

class MyVolAction : public GeoVolumeAction
{
 public:
  MyVolAction();
  ~MyVolAction();

  // Callback method.
  void handleVPhysVol (const GeoVPhysVol *);
};

#endif
