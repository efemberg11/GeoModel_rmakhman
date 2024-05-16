/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELKERNEL_GeoVSurface_H
#define GEOMODELKERNEL_GeoVSurface_H

/**
 * @class GeoVSurface
 * 
 * @brief An abstract class of virtual surfaces
 *
 */

#include "GeoModelKernel/GeoPlacement.h"
#include "GeoModelKernel/GeoNodePositioning.h"
#include "GeoModelKernel/GeoDefinitions.h"


class GeoVSurface : public GeoPlacement, public GeoNodePositioning {
 public:
    GeoVSurface():
      GeoNodePositioning{this}{
    }
  
  virtual double area() const=0;	// calculate the area of a surface
  
  // method from GeoPlacement

  
 protected:
  virtual ~GeoVSurface() = default;  
};


#endif
