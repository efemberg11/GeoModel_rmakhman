/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPHYSVOL_H
#define GEOMODELKERNEL_GEOPHYSVOL_H

#include "GeoModelKernel/GeoVPhysVol.h"

/**
 * @class GeoPhysVol
 *
 * @brief A GeoPhysVol is a physical volume for normal recursive
 * access only. This means, normally, that it represents a
 * piece of detector that nobody cares about except for
 * tracing engines like GEANT. More specifically:
 *
 * The position of the physical volume is not kept with
 * the physical volume. It can easily be obtained by
 * traversing detector graph (ComputeTransformAction), but
 * it cannot be obtained in constant time. If you need to
 * know the absolute default and/or aligned position of the
 * detector, you need to use a different kind of physical
 * volume called a GeoRAPhysicalVolume, which has a higher
 * memory cost.
 */


class GeoPhysVol : public GeoVPhysVol{
  public:
      GeoPhysVol(const GeoLogVol* LogVol);

      /// Executes a GeoNodeAction.
      void exec(GeoNodeAction *action) const override final;
  protected:
      virtual ~GeoPhysVol() = default;

};


#endif
