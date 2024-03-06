/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Units.h"
#include <cmath>

namespace {
  constexpr double rotTolerance = 0.01 * GeoModelKernelUnits::deg;

  inline double cutOff(const double val, const double cutOff) {
     return std::abs(val) > cutOff ? val : 0.;
  }
  inline double calcAngle(const double y, const double x) {
    return cutOff(std::atan2(cutOff(y, std::numeric_limits<float>::epsilon()), 
                             cutOff(x, std::numeric_limits<float>::epsilon())), rotTolerance);
  }
}

namespace GeoTrf {
  

  GeoRotation::GeoRotation(double phi, double theta, double psi):
      GeoRotation{EulerAngles{phi, theta, psi}} {}

  GeoRotation::GeoRotation(const EulerAngles& angles):
      RotationMatrix3D{get3DRotMatZ(angles.phi) * 
                       get3DRotMatX(angles.theta) *
                       get3DRotMatZ(angles.psi)} {}
      

  GeoRotation::GeoRotation(const CoordEulerAngles& angles):
      RotationMatrix3D{get3DRotMatZ(angles.gamma) *
                       get3DRotMatY(angles.beta)*
                       get3DRotMatX(angles.alpha)} {}
  
  
  
  CoordEulerAngles getCoordRotationAngles(const Transform3D& trans) {
    return getCoordRotationAngles(trans.linear());
  }
  CoordEulerAngles getCoordRotationAngles(const RotationMatrix3D& rot) {
      CoordEulerAngles toRet{};
      const double beta1 = -std::asin(std::clamp(rot(2,0), -1., 1.));
      const double beta2 = M_PI - beta1;
      /// Choose the beta value where cos(\beta) > 0
      toRet.beta = cutOff(std::cos(beta1) >= 0. ? beta1 : beta2, rotTolerance);
      /// Check that cos(beta) != 0.
      const double cosBeta = std::cos(toRet.beta);
      if (std::abs(cosBeta) > std::numeric_limits<float>::epsilon()) {
          const double iCosBeta = 1./ cosBeta;
          toRet.gamma = calcAngle(rot(1,0) * iCosBeta, rot(0,0) * iCosBeta);
          toRet.alpha = calcAngle(rot(2,1) * iCosBeta, rot(2,2) * iCosBeta);
      } else {
        toRet.beta = rot(2,0) > 0 ? -M_PI_2  : M_PI_2;
        toRet.alpha = rot(2,0) < 0 ? calcAngle(rot(0,1), rot(0,2)) :
                                     calcAngle(-rot(0,1), -rot(0,2));
      }
      return toRet;
   }
   EulerAngles getGeoRotationAngles(const Transform3D& trans) {
     return getGeoRotationAngles(trans.linear());
   }
   EulerAngles getGeoRotationAngles(const RotationMatrix3D& rotMat){
      EulerAngles toRet{};
      toRet.theta = cutOff(std::acos(std::clamp(rotMat(2,2),-1., 1.)), rotTolerance);
      const double sinTheta = std::sin(toRet.theta);
      if (std::abs(sinTheta) > std::numeric_limits<float>::epsilon()) {
        toRet.phi = calcAngle(sinTheta* rotMat(0,2), -sinTheta * rotMat(1,2));
        toRet.psi = calcAngle(sinTheta *rotMat(2,0),  sinTheta * rotMat(2,1));   
      } else {
        toRet.theta = rotMat(2,2) > 0 ? 0. : M_PI;
        toRet.phi = calcAngle(rotMat(1,0), rotMat(0,0));
      }

      return toRet;
   }
 

  
  int EulerAngles::compare(const EulerAngles& other) const {
      if (std::abs(phi - other.phi) > rotTolerance) return phi < other.phi ? -1 : 1;
      if (std::abs(theta - other.theta) > rotTolerance) return theta < other.theta ? -1 : 1;
      if (std::abs(psi - other.psi) > rotTolerance) return psi < other.psi ? -1 : 1;
      return 0;
  }
  bool EulerAngles::operator<(const EulerAngles& other) const {
     return compare(other) < 0;
  }
  EulerAngles::operator bool() const {
     return std::abs(phi) > rotTolerance || std::abs(theta) > rotTolerance || std::abs(psi) > rotTolerance;
  }

  int CoordEulerAngles::compare(const CoordEulerAngles& other) const {
      if (std::abs(alpha - other.alpha) > rotTolerance) return alpha < other.alpha ? -1 : 1;
      if (std::abs(beta - other.beta) > rotTolerance) return beta < other.beta ? -1 : 1;
      if (std::abs(gamma - other.gamma) > rotTolerance) return gamma < other.gamma ? -1 : 1;
      return 0;
  }
  bool CoordEulerAngles::operator<(const CoordEulerAngles& other) const {
     return compare(other) < 0;
  }
  CoordEulerAngles::operator bool() const {
     return std::abs(alpha) > rotTolerance || std::abs(beta) > rotTolerance || std::abs(gamma) > rotTolerance;
  }

}

