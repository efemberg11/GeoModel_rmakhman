/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOFULLPHYSVOL_H
#define GEOMODELKERNEL_GEOFULLPHYSVOL_H

#include "GeoModelKernel/GeoVFullPhysVol.h"
#include <vector>
#include <string>

class GeoLogVol;
class GeoVAlignmentStore;

/**
 * @class GeoFullPhysVol
 *
 * @brief This class represent a physical volume whose absolute
 * default and aligned positions are known and held in cache.
 *
 * These pieces should not be part of a subtree which is used as a
 * shared instance. For if it is, then of course the absolute position 
 * of the piece is an absurdity. The run time error will result as 
 * soon as the absolute position is requested.
 *
 * These elements may be declared to readout geometry. See the class 
 * ReadoutElement.
 */

class GeoFullPhysVol final : public GeoVFullPhysVol {
 
  public:
    GeoFullPhysVol (const GeoLogVol* LogVol);

 
  /// Executes a GeoNodeAction.
  virtual void exec(GeoNodeAction *action) const override final;

 
  /// Meaning of the input parameter 'attached'
  /// TRUE: all cloned volumes are meant to stay identical to their clone origin for the lifetime
  ///       further changes are permitted neither in the origin nor in the clone results
  ///
  /// FALSE: use this value if you expect further changes in either clone origing or its clone results
  ///        which don't need to be syncronized. The clone origin and its clone are identical ONLY by
  ///        the time of cloning, further identity is not guaranteed
  GeoFullPhysVol* clone(bool attached = true);

  const GeoFullPhysVol* cloneOrigin() const;

  /// The following method breaks consistency of cloned volumes!
  /// Use it only in Simulation jobs and
  /// don't call it until geometry has been completely translated to G4
  void clear(); // drop subtree


  private:
    virtual ~GeoFullPhysVol() = default;
    const GeoFullPhysVol* m_cloneOrigin{nullptr};
};

#endif
