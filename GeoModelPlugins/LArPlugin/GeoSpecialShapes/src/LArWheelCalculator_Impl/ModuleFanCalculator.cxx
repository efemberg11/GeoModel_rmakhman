/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "ModuleFanCalculator.h"
#include "GeoSpecialShapes/LArWheelCalculator.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits


#ifdef HARDDEBUG
#undef HARDDEBUG
#endif

namespace LArWheelCalculator_Impl
{

  ModuleFanCalculator::ModuleFanCalculator(LArWheelCalculator* lwc)
    : m_lwc(lwc)
  {
  }

  double ModuleFanCalculator::DistanceToTheNearestFan(GeoTrf::Vector3D &p, int & out_fan_number) const
  {
    return 0.0;
    // Dummy implementation
    // Requires proper migration from CLHEP to Eigen
  }

  int ModuleFanCalculator::PhiGapNumberForWheel(int i) const {
    i += lwc()->m_ZeroGapNumber;
    i -= lwc()->m_LastFan / 2;
    if(i < 0) i += lwc()->m_NumberOfFans;
    if(i >= lwc()->m_NumberOfFans) i -= lwc()->m_NumberOfFans;
    return i;
  }

  std::pair<int, int> ModuleFanCalculator::GetPhiGapAndSide(const GeoTrf::Vector3D &p) const
  {
    return std::pair<int,int>(0,0);
    // Dummy implementation
    // Requires proper migration from CLHEP to Eigen
    
  }

}
