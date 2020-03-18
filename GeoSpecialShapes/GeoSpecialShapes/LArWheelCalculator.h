/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOSPECIALSHAPES_LARWHEELCALCULATOR_H
#define GEOSPECIALSHAPES_LARWHEELCALCULATOR_H

#include <vector>

#include "CLHEP/Vector/ThreeVector.h"
//#ifndef XAOD_STANDALONE
//    #include "AthenaKernel/CLASS_DEF.h"
//#endif // XAOD_STANDALONE

#include "GeoSpecialShapes/LArWheelCalculatorEnums.h"
// Physical constants
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#define LARWC_SINCOS_POLY 5
#define LARWC_DTNF_NEW

class IRDBRecordset;
//class RDBParamRecords;

//#define HARDDEBUG

// Forward declarations
namespace LArWheelCalculator_Impl {
  class IDistanceCalculator;
  class DistanceCalculatorSaggingOff;
  class DistanceCalculatorSaggingOn;

  class IFanCalculator;
  class ModuleFanCalculator;
  template <typename SaggingType> class WheelFanCalculator;
  template <typename SaggingType> class DistanceToTheNeutralFibre_OfFan;
}

/// @class LArWheelCalculator
/// This class separates some of the geometry details of the LAr
/// endcap.
/// 26-May-2009 AMS: remove all previous comments from here as obsoleted
///
class LArWheelCalculator
{

    friend class LArWheelCalculator_Impl::DistanceCalculatorSaggingOff;
    friend class LArWheelCalculator_Impl::DistanceCalculatorSaggingOn;
    friend class LArWheelCalculator_Impl::ModuleFanCalculator;
    template <typename SaggingType> friend class LArWheelCalculator_Impl::WheelFanCalculator;
    template <typename SaggingType> friend class LArWheelCalculator_Impl::DistanceToTheNeutralFibre_OfFan;

  public:

    LArWheelCalculator(LArG4::LArWheelCalculator_t a_wheelType, int zside = 1);
    virtual ~LArWheelCalculator();

    LArWheelCalculator (const LArWheelCalculator&) = delete;
    LArWheelCalculator& operator= (const LArWheelCalculator&) = delete;

    static const char *LArWheelCalculatorTypeString(LArG4::LArWheelCalculator_t);
    static double GetFanHalfThickness(LArG4::LArWheelCalculator_t);

    // "Get constant" methods:
    double GetWheelThickness() const { return m_WheelThickness; }
    double GetdWRPtoFrontFace() const { return m_dWRPtoFrontFace; }
    double GetStraightStartSection() const { return m_StraightStartSection; }
    virtual LArG4::LArWheelCalculator_t type() const { return m_type; }
    // "zShift" is the z-distance (cm) that the EM endcap is shifted
    // (due to cabling, etc.)
    int GetAtlasZside() const { return m_AtlasZside; }
    double zShift() const { return m_zShift; }
    double GetFanFoldRadius() const { return m_FanFoldRadius; }
    double GetZeroFanPhi() const { return m_ZeroFanPhi; }
    int GetNumberOfWaves() const { return m_NumberOfWaves; }
    int GetNumberOfHalfWaves() const { return m_NumberOfHalfWaves; }
    int GetNumberOfFans() const { return m_NumberOfFans; }

    double GetActiveLength() const { return m_ActiveLength; }
    double GetFanStepOnPhi() const { return m_FanStepOnPhi; }
    double GetHalfWaveLength() const { return m_HalfWaveLength; }
    double GetQuarterWaveLength() const { return m_QuarterWaveLength; }
    double GetWheelRefPoint() const { return m_zWheelRefPoint; }
    double GetFanHalfThickness() const { return m_FanHalfThickness; }

    bool GetisModule() const { return m_isModule; }
    bool GetisElectrode() const { return m_isElectrode; }
    bool GetisInner() const { return m_isInner; }
    bool GetisBarrette() const { return m_isBarrette; }
    bool GetisBarretteCalib() const { return m_isBarretteCalib; }

    double GetWheelInnerRadius(double *) const;
    void GetWheelOuterRadius(double *) const;

    double GetElecFocaltoWRP() const { return m_dElecFocaltoWRP; }
    // "set constant" method:

    int GetFirstFan() const { return m_FirstFan; }
    int GetLastFan() const { return m_LastFan; }

    int GetStartGapNumber() const { return m_ZeroGapNumber; }
    void SetStartGapNumber(int n) { m_ZeroGapNumber = n; }

    /// @name geometry methods
    /// @{

    /// Determines the nearest to the input point fan.
    /// Rotates point p to the localFan coordinates and returns the
    /// fan number to out_fan_number parameter.
    double DistanceToTheNearestFan(CLHEP::Hep3Vector &p, int & out_fan_number) const;

    /// Calculates aproximate, probably underestimate, distance to the
    /// neutral fibre of the vertical fan. Sign of return value means
    /// side of the fan; negative - lower phi.
    double DistanceToTheNeutralFibre(const CLHEP::Hep3Vector &p, int fan_number) const;

    CLHEP::Hep3Vector NearestPointOnNeutralFibre(const CLHEP::Hep3Vector &p,
                                                 int fan_number) const;
    std::vector<double> NearestPointOnNeutralFibre_asVector(const CLHEP::Hep3Vector &p,
                                                            int fan_number) const;
    int GetPhiGap(const CLHEP::Hep3Vector &p) const { return GetPhiGapAndSide(p).first; }
    int PhiGapNumberForWheel(int) const;
    std::pair<int, int> GetPhiGapAndSide(const CLHEP::Hep3Vector &p) const;
    double AmplitudeOfSurface(const CLHEP::Hep3Vector& P, int side, int fan_number) const;

    /// @}

  private:
    LArG4::LArWheelCalculator_t m_type;
    
    int m_AtlasZside;
    bool m_SaggingOn; // !
    bool m_phiRotation;
    bool m_slant_use_default;
    double m_slant_parametrization[5]; // pol4
    double m_sin_parametrization[7]; // up to pol6
    double m_cos_parametrization[7];
    std::vector<std::vector<double> > m_sagging_parameter; // !
    //double m_WheelThickness;
    // double m_HalfWheelThickness;
    //double m_zWheelFrontFace, m_zWheelBackFace;
    
    // N.B. all const values copied from the DB@
    // https://atlas-geometry-db.web.cern.ch/atlas-geometry-db/
    const double m_ActiveLength         = 510   * SYSTEM_OF_UNITS::mm;   //mm
    const double m_StraightStartSection = 2     * SYSTEM_OF_UNITS::mm;   //mm
    const double m_dWRPtoFrontFace      = 11    * SYSTEM_OF_UNITS::mm;   //mm
    const double m_HalfGapBetweenWheels = .15   * SYSTEM_OF_UNITS::cm;   //cm
    const double m_zWheelRefPoint       = 368.95* SYSTEM_OF_UNITS::cm;   //cm
    const double m_dMechFocaltoWRP      = 369.1 * SYSTEM_OF_UNITS::cm;   //cm
    const double m_dElecFocaltoWRP      = 368.9 * SYSTEM_OF_UNITS::cm;   //cm
    const double m_rOuterCutoff         = 203.4 * SYSTEM_OF_UNITS::cm;   //cm
    const double m_eta_hi               = 3.2 ;  
    const double m_eta_mid              = 2.5 ; 
    const double m_eta_low              = 1.375;
    const double m_zShift               = 4     * SYSTEM_OF_UNITS::cm;   //cm
    
    const double m_WheelThickness = m_ActiveLength + 2.*m_StraightStartSection;
    const double m_HalfWheelThickness = m_WheelThickness * 0.5;
    const double m_zWheelFrontFace = m_dMechFocaltoWRP + m_dWRPtoFrontFace;
    const double m_zWheelBackFace = m_zWheelFrontFace + m_WheelThickness;
    
    /////
    double m_QuarterWaveLength;
    double m_HalfWaveLength;
    double m_FanFoldRadius;
    double m_ZeroFanPhi;
    double m_ZeroFanPhi_ForDetNeaFan;
    double m_FanStepOnPhi;
    int m_NumberOfWaves;
    int m_NumberOfHalfWaves;
    int m_NumberOfFans;
    //int m_HalfNumberOfFans; removed because unused. DM 2015-07-30
    double m_FanHalfThickness;
    int m_ZeroGapNumber;
    int m_FirstFan;
    int m_LastFan;

    bool m_isModule;
    bool m_isElectrode;
    bool m_isInner;
    bool m_isBarrette;
    bool m_isBarretteCalib;

    // int m_fan_number; // break thread-safety -> removed DM 2015-07-30

    void outer_wheel_init();
    void inner_wheel_init();
    void module_init();

  public:

    /*void set_m_fan_number(const int &fan_number)
    {
      m_fan_number = fan_number;
      if(m_fan_number < 0) m_fan_number += m_NumberOfFans;
      m_fan_number += m_ZeroGapNumber;
      if(m_fan_number >= m_NumberOfFans) m_fan_number -= m_NumberOfFans;
    }*/
    int adjust_fan_number(int fan_number) const {
      int res_fan_number = fan_number;
      if(res_fan_number < 0) res_fan_number += m_NumberOfFans;
      res_fan_number += m_ZeroGapNumber;
      if(res_fan_number >= m_NumberOfFans) res_fan_number -= m_NumberOfFans;
      return res_fan_number;
    }

    /// Calculates wave slant angle using parametrization for current wheel
    /// for given distance from calorimeter axis
    double parameterized_slant_angle(double) const;

  private:

    void parameterized_sincos(const double, double &, double &) const;
    void parameterized_sin(const double, double &, double &) const;

  private:

    LArWheelCalculator_Impl::IDistanceCalculator *m_distanceCalcImpl;
    LArWheelCalculator_Impl::IFanCalculator *m_fanCalcImpl;

    void fill_sincos_parameterization();
};

//#ifndef XAOD_STANDALONE
    //using the macro below we can assign an identifier (and a version)
    //This is required and checked at compile time when you try to record/retrieve
//    CLASS_DEF(LArWheelCalculator , 900345678 , 1)
//#endif // XAOD_STANDALONE

#endif // GEOSPECIALSHAPES_LARWHEELCALCULATOR_H
