/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "DistanceCalculatorSaggingOn.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include <vector>
#include <stdexcept>
#include <iostream>

#include "GeoSpecialShapes/LArWheelCalculator.h"

namespace LArWheelCalculator_Impl
{
  DistanceCalculatorSaggingOn::DistanceCalculatorSaggingOn(const std::string& saggingOptions,
                                                           LArWheelCalculator* lwc)
    : parent(lwc),
      m_saggingOptions(saggingOptions)
  {
    // load sagging parameters
    /*RDBParamReader rdbr(rdbAccess);
      rdbr.
      data( "EmecMagicNumbers", larVersionKey.tag(), larVersionKey.node() ).
      fallback_to( "EmecMagicNumbers","EMECMagigNumbers-00" ).
      param(lwc()->m_ActiveLength, "ACTIVELENGTH", mm).
      param(lwc()->m_dWRPtoFrontFace, "REFTOACTIVE", mm).
      param(lwc()->m_StraightStartSection, "STRAIGHTSTARTSECTION", mm)
      ;*/
    init_sagging_parameters();
  }
	
  void DistanceCalculatorSaggingOn::init_sagging_parameters()
  {
    std::string sagging_opt_value = m_saggingOptions;
    m_sagging_parameter.resize (lwc()->m_NumberOfFans, std::vector<double> (5, 0.));
    //	if(m_SaggingOn) {
    if(sagging_opt_value.substr(0, 4) == "file"){
      std::string sag_file = sagging_opt_value.substr(5);
      std::cout
        << "geting sagging parameters from file "
        << sag_file << " ..." << std::endl;
      FILE *F = fopen(sag_file.c_str(), "r");
      if(F == 0){
	std::cerr
          << "cannot open EMEC sagging parameters file "
          << sag_file
          << std::endl;
        throw std::runtime_error("LArWheelCalculator: read sagging parameters from file");
      }
      int s, w, t, n;
      double p0, p1, p2, p3, p4;
      while(!feof(F) &&
            fscanf(F, "%80d %80d %80d %80d %80le %80le %80le %80le %80le",
                   &s, &w, &t, &n, &p0, &p1, &p2, &p3, &p4) == 9)
      {
        if(s == lwc()->m_AtlasZside &&
           ((w == 0 && lwc()->m_isInner) || (w == 1 && !lwc()->m_isInner)) &&
           ((t == 0 && !lwc()->m_isElectrode) || (t == 1 && lwc()->m_isElectrode)) &&
           (n >= 0 && n < lwc()->m_NumberOfFans))
        {
          m_sagging_parameter[n][0] = p0;
          m_sagging_parameter[n][1] = p1;
          m_sagging_parameter[n][2] = p2;
          m_sagging_parameter[n][3] = p3;
          m_sagging_parameter[n][4] = p4;
	  std::cout
              << "sagging for " << s << " " << w << " " << t
              << " " << n << ": " << p0 << " " << p1 << " "
              << p2 << " " << p3 << std::endl;
        }
      }
      fclose(F);
    } else {
      double a, b, c, d;
      if(sscanf(sagging_opt_value.c_str(), "%80le %80le %80le %80le", &a, &b, &c, &d) != 4){
	std::cerr
            << "wrong value(s) "
            << " for EMEC sagging parameters: "
            << sagging_opt_value << ", defaults are used" << std::endl;
      } else {
        for(int j = 0; j < lwc()->m_NumberOfFans; j ++){
          if(lwc()->m_isInner){
            m_sagging_parameter[j][1] = a;
            m_sagging_parameter[j][0] = b * SYSTEM_OF_UNITS::mm;
          } else {
            m_sagging_parameter[j][1] = c;
            m_sagging_parameter[j][0] = d * SYSTEM_OF_UNITS::mm;
          }
        }
      }
    }
    //	}
    std::cout  << "Sagging parameters        : " << m_sagging_parameter[0][0] << " " << m_sagging_parameter[0][1] << std::endl
        << "Sagging parameters        : " << m_sagging_parameter[1][0] << " " << m_sagging_parameter[1][1] << std::endl;
  }

  // Represents aproximate, probably underestimate, distance to the
  // neutral fibre of the vertical fan. Sign of return value means
  // side of the fan; negative - lower phi.
  //
  // Uses m_fan_number to compute sagging.
  double DistanceCalculatorSaggingOn::DistanceToTheNeutralFibre(const GeoTrf::Vector3D &p, int fan_number) const {
    GeoTrf::Vector3D sagging_corrected( p.x()+get_sagging(p, fan_number), p.y(), p.z() );
    return parent::DistanceToTheNeutralFibre(sagging_corrected, fan_number);
  }

  GeoTrf::Vector3D DistanceCalculatorSaggingOn::NearestPointOnNeutralFibre(const GeoTrf::Vector3D &p, int fan_number) const {
    GeoTrf::Vector3D sagging_corrected( p.x()+get_sagging(p, fan_number), p.y(), p.z() );
    return parent::NearestPointOnNeutralFibre(sagging_corrected, fan_number);
  }

  double DistanceCalculatorSaggingOn::AmplitudeOfSurface(const GeoTrf::Vector3D& p, int side, int fan_number) const {
    return parent::AmplitudeOfSurface(p, side, fan_number) - get_sagging(p, fan_number);
  }


  // the function uses m_fan_number for phi-dependent sagging computation
  double DistanceCalculatorSaggingOn::get_sagging(const GeoTrf::Vector3D &P, int fan_number) const {
#ifdef HARDDEBUG
    std::cout << "get_sagging: MFN = " << fan_number << std::endl;
#endif
    double dx = P.z() / lwc()->m_HalfWheelThickness - 1.;
    dx *= dx;
    dx = 1. - dx;
    //dx *= SaggingAmplitude * sin(FanStepOnPhi * m_fan_number + ZeroFanPhi);
    //int n = m_fan_number;
    //if(n < 0) n += m_NumberOfFans;
    //n += m_ZeroGapNumber;
    //if(n >= m_NumberOfFans) n -= m_NumberOfFans;
    //const std::vector<double>& sp = m_sagging_parameter[n];
    const std::vector<double>& sp = m_sagging_parameter[fan_number];
    double R = sqrt(P.x()*P.x()+P.y()*P.y()+P.z()*P.z()) / SYSTEM_OF_UNITS::mm;  // Requires proper migration from CLHEP to Eigen!
    double r = R;
    double result = sp[0];
    result += R * sp[1];
    R *= r;
    result += R * sp[2];
    R *= r;
    result += R * sp[3];
    R *= r;
    result += R * sp[4];

#ifdef HARDDEBUG
    /*printf("get_sagging: (%6.3f, %6.3f, %6.3f) %6.3f; MFN %4d;"
      "n %3d; sp %6.4f %6.4f; dx %6.3f; %.6f\n", P.x()/mm, P.y()/mm, P.z()/mm,
      r, m_fan_number, n, sp[0], sp[1], dx, result*dx);*/
    printf("get_sagging: (%6.3f, %6.3f, %6.3f) %6.3f;"
           " sp %6.4f %6.4f; dx %6.3f; %.6f\n", P.x()/SYSTEM_OF_UNITS::mm, P.y()/SYSTEM_OF_UNITS::mm, P.z()/SYSTEM_OF_UNITS::mm,
           r, sp[0], sp[1], dx, result*dx);
#endif //HARDDEBUG

    return result * dx;
  }

}
