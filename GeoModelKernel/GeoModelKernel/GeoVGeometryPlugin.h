/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVGEOMETRYPLUGIN_H
#define GEOMODELKERNEL_GEOVGEOMETRYPLUGIN_H

/**
 * @class: GeoVGeometryPlugin
 *
 * @brief Abstact Base Class for a geometry plugin.  The geometry
 *	plugin builds the raw geometry, and publishes a list of
 *      of alignable transforms and full physical volumes 
 */

#include "GeoModelKernel/GeoVPhysVol.h"

class GeoPhysVol;
class GeoVGeometryPlugin 
{
 public:
  GeoVGeometryPlugin();
  virtual ~GeoVGeometryPlugin();

  //	Create the system.
  virtual void create (GeoPhysVol* world) = 0;

 private:
  GeoVGeometryPlugin(const GeoVGeometryPlugin &right)=delete;
  GeoVGeometryPlugin & operator=(const GeoVGeometryPlugin &right)=delete;

};

#endif
