/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef LARWHEELCALCULATOR_IMPL_WHEELFANCALCULATOR_H
#define LARWHEELCALCULATOR_IMPL_WHEELFANCALCULATOR_H

#include "IFanCalculator.h"
#include "GeoSpecialShapes/LArWheelCalculator.h"

#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#ifdef HARDDEBUG
#undef HARDDEBUG
#endif

namespace LArWheelCalculator_Impl
{

  // mode marker classes
  class SaggingOn_t {};
  class SaggingOff_t {};

  template <typename SaggingType>
  class DistanceToTheNeutralFibre_OfFan {};

  template<>
  class DistanceToTheNeutralFibre_OfFan<SaggingOn_t>
  {
    public:
      static inline double calculate(const LArWheelCalculator* lwc, int fan_number,     GeoTrf::Vector3D &p) {
        //lwc->set_m_fan_number(fan_number);
        return lwc->DistanceToTheNeutralFibre(p, lwc->adjust_fan_number(fan_number));
      }
  };

  template<> class DistanceToTheNeutralFibre_OfFan<SaggingOff_t>
  {
    public:
      static inline double calculate(const LArWheelCalculator* lwc, int /*fan_number*/, GeoTrf::Vector3D &p) {
        // saggingOff distance calculations does not use fan_number, use arbitrary recognisible magic number
        return lwc->DistanceToTheNeutralFibre(p, -531135);
      }
  };

  enum FanSearchDirection_t {
    FORWARD = 1,    // delta =  1
    BACKWARD = -1   // delta = -1
  };


  template <typename SaggingType, FanSearchDirection_t dir >
  class StepFan {};


  template <FanSearchDirection_t dir >
  class StepFan<SaggingOff_t, dir>
  {
    public:
      static inline void next(int &/*fan_number*/) {}
      static inline void adjust(int &/*fan_number*/) {}
  };


  template <>
  class StepFan<SaggingOn_t, FORWARD>
  {
    public:
      static inline void next(int &fan_number) {
        fan_number++;
      }
      static inline void adjust(int &fan_number) {
        fan_number--;
      }
  };

  template <>
  class StepFan<SaggingOn_t, BACKWARD>
  {
    public:
      static inline void next(int &fan_number) {
        fan_number--;
      }
      static inline void adjust(int &/*fan_number*/) {}
  };

  template <FanSearchDirection_t dir>
  class DoSearch {};

  template <>
  class DoSearch<FORWARD>
  {
    public:
      template <typename T >
        static inline bool pred(T val) {
          return (val > 0.);
        }
  };

  template <>
  class DoSearch<BACKWARD>
  {
    public:
      template <typename T >
        static inline bool pred(T val) {
          return (val < 0.);
        }
  };

  /// @todo Needs documentation
  template <typename SaggingType, FanSearchDirection_t dir, class NFDistance >
  inline void rotate_to_nearest_fan(const LArWheelCalculator* lwc, int &fan_number,
                                    const double angle, GeoTrf::Vector3D &p)
  {
    // Dummy implementation
    // Requires proper migration from CLHEP to Eigen
  }


  /// LAr wheel fan calculator, templated for sagging settings.
  ///
  template <typename SaggingType>
  class WheelFanCalculator : public IFanCalculator
  {
    public:
      WheelFanCalculator(LArWheelCalculator* lwc)
        : m_lwc(lwc)
      {
      }

      /// @name Geometry methods
      /// @{

      virtual double DistanceToTheNearestFan(GeoTrf::Vector3D &p, int & out_fan_number) const
      {
	return 0.0;
	// Dummy implementation
	// Requires proper migration from CLHEP to Eigen
	
      }

      virtual int PhiGapNumberForWheel(int i) const
      {
        return i;
      }

      virtual std::pair<int, int> GetPhiGapAndSide(const GeoTrf::Vector3D &p) const
      {
        return std::pair<int, int>(0, 0);
	// Dummy implementation
	// Requires proper migration from CLHEP to Eigen
      }

      /// @}

      inline const LArWheelCalculator *lwc() const { return m_lwc; };

    private:
      LArWheelCalculator* m_lwc;

  };

}

#endif // LARWHEELCALCULATOR_IMPL_WHEELFANCALCULATOR_H
