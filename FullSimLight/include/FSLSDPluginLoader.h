/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef FSLSDPLUGINLOADER_H_
#define FSLSDPLUGINLOADER_H_
#include "FullSimLight/FSLSensitiveDetectorPlugin.h"
#include "GeoModelKernel/GeoPluginLoader.h"
typedef GeoPluginLoader<FSLSensitiveDetectorPlugin> FSLSDPluginLoader;
#endif
