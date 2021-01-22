/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef __LArWheelCalculator_Impl_DistanceCalculatorSaggingOff_H__
#define __LArWheelCalculator_Impl_DistanceCalculatorSaggingOff_H__


#include "IDistanceCalculator.h"

class LArWheelCalculator;

namespace LArWheelCalculator_Impl
{

  /// @class DistanceCalculatorSaggingOff
  /// @brief Implements details of distance calculation to parts of the
  /// LAr endcap without sagging corrections.
  ///
  class DistanceCalculatorSaggingOff : public IDistanceCalculator
  {

  public:
    
    /// Constructor
    DistanceCalculatorSaggingOff(LArWheelCalculator* lwc);

    /// @name Geometry methods
    /// @{
    virtual double DistanceToTheNeutralFibre(const GeoTrf::Vector3D &p, int fan_number) const;
    virtual double DistanceToTheNeutralFibre_ref(const GeoTrf::Vector3D &p, int fan_number) const;
    virtual GeoTrf::Vector3D NearestPointOnNeutralFibre(const GeoTrf::Vector3D &p, int fan_number) const;
    virtual GeoTrf::Vector3D NearestPointOnNeutralFibre_ref(const GeoTrf::Vector3D &p, int fan_number) const;
    virtual double AmplitudeOfSurface(const GeoTrf::Vector3D& P, int side, int fan_number) const;
    /// @}
    
    /// Return the calculator:
    inline const LArWheelCalculator *lwc() const { return m_lwc; };
    
  private:
    
    LArWheelCalculator* m_lwc;
    double m_EndQuarterWave;
    
  };
  
}

#endif // __LArWheelCalculator_Impl_IDistanceCalculatorOff_H__
