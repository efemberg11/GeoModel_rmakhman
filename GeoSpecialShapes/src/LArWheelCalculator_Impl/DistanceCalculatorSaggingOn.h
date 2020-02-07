/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef __LArWheelCalculator_Impl_DistanceCalculatorSaggingOn_H__
#define __LArWheelCalculator_Impl_DistanceCalculatorSaggingOn_H__

#include "DistanceCalculatorSaggingOff.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

class LArWheelCalculator;

namespace LArWheelCalculator_Impl
{

  /// @class DistanceCalculatorSaggingOn
  /// @brief Implements details of distance calculation to parts of the
  /// LAr endcap with sagging taken into account.
  ///
  class DistanceCalculatorSaggingOn : public DistanceCalculatorSaggingOff
  {

    public:

      typedef DistanceCalculatorSaggingOff parent;

      /// Constructor
      DistanceCalculatorSaggingOn(const std::string& saggingOptions,
                                  LArWheelCalculator* lwc);

      /// @name Geometry methods
      /// @{
      virtual double DistanceToTheNeutralFibre(const GeoTrf::Vector3D &p, int fan_number) const;
      virtual GeoTrf::Vector3D NearestPointOnNeutralFibre(const GeoTrf::Vector3D &p, int fan_number) const;
      virtual double AmplitudeOfSurface(const GeoTrf::Vector3D& P, int side, int fan_number) const;
      /// @}

    private:

      double get_sagging(const GeoTrf::Vector3D &P, int fan_number) const;
      void init_sagging_parameters();

      std::vector<std::vector<double> > m_sagging_parameter;
      std::string m_saggingOptions;

  };

}

#endif // __LArWheelCalculator_Impl_IDistanceCalculatorOn_H__
