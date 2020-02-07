/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef __LArWheelCalculator_Impl_IDistanceCalculator_H__
#define __LArWheelCalculator_Impl_IDistanceCalculator_H__

#include "GeoModelKernel/GeoDefinitions.h"

namespace LArWheelCalculator_Impl
{

  /// @class IDistanceCalculator
  /// Abstract interface for calculator classes that handle distance
  /// calculation to parts of the LAr endcap.
  ///
  class IDistanceCalculator
  {

    public:

      /// Virtual destructor
      virtual ~IDistanceCalculator() {};

      /// @name Geometry methods
      /// @{

      virtual double DistanceToTheNeutralFibre(const GeoTrf::Vector3D &p,
                                               int fan_number) const = 0;

      virtual GeoTrf::Vector3D NearestPointOnNeutralFibre(const GeoTrf::Vector3D &p,
                                                           int fan_number) const = 0;

      virtual double AmplitudeOfSurface(const GeoTrf::Vector3D& p, int side,
                                        int fan_number) const = 0;

      /// @}

  };

}

#endif // __LArWheelCalculator_Impl_IDistanceCalculator_H__
