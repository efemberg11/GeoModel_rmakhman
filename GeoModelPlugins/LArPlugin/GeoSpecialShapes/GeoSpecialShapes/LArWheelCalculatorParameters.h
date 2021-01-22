/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOSPECIALSHAPES_LARWHEELCALCULATORPARAMETERS_H
#define GEOSPECIALSHAPES_LARWHEELCALCULATORPARAMETERS_H

struct LArWheelCalculatorParameters
{
  double m_zWheelRefPoint;
  double m_dMechFocaltoWRP;
  double m_dElecFocaltoWRP;
  double m_HalfGapBetweenWheels;
  double m_rOuterCutoff;
  double m_zShift;
  double m_eta_hi;
  double m_eta_mid;
  double m_eta_low;
  double m_ActiveLength;
  double m_StraightStartSection;
  double m_dWRPtoFrontFace;
  std::string m_pr_opt_value;
  std::string m_sagging_opt_value;
  std::string m_slant_params_inner;
  std::string m_slant_params_outer;
  int m_NumberOfFans1;
  int m_NumberOfWaves1;
  int m_NumberOfFans2;
  int m_NumberOfWaves2;
};

#endif
