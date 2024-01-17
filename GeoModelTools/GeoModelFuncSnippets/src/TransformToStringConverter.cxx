/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelFuncSnippets/TransformToStringConverter.h"
#include "GeoModelKernel/Units.h"

namespace{
    constexpr double toDeg = 1./ GeoModelKernelUnits::deg;
}

namespace GeoTrf{
    std::string toString(const Transform3D& transform, bool useCoordAngles, int precision) {
        std::stringstream ostr{};
        ostr << std::setiosflags(std::ios::fixed) << std::setprecision(precision);
        const Vector3D trans = transform.translation();
        bool printed{false};
        if (std::sqrt(trans.dot(trans)) > 0.1 *GeoModelKernelUnits::micrometer) {
            ostr<<"translation -- ";
            ostr<<toString(trans, precision);
            printed = true;
        }
        if (useCoordAngles) {
            CoordEulerAngles angles = getCoordRotationAngles(transform);
            if (angles) {
                if (printed) ostr<<", ";
                ostr<<"Euler angles -- ";
                ostr<<toString(angles, precision); 
                printed = true;               
            }
        } else {
            EulerAngles angles = getGeoRotationAngles(transform);
            if (angles){
                 if(printed) ostr<<", ";
                 ostr<<"Euler angles -- ";
                 ostr<<toString(angles, precision);
                 printed = true;                 
            }
        }

        if (!printed) ostr<<"Identity transformation";
        return ostr.str();
    }  
    std::string toString(const EulerAngles& angles, int precision) {
        std::stringstream ostr{};
        ostr << std::setiosflags(std::ios::fixed) << std::setprecision(precision);
        ostr<<"phi: "<<angles.phi *toDeg<<", ";
        ostr<<"theta: "<<angles.theta *toDeg<<", ";
        ostr<<"psi: "<<angles.psi *toDeg;
        return ostr.str();
    }
    std::string toString(const CoordEulerAngles& angles, int precision) {
        std::stringstream ostr{};
        ostr << std::setiosflags(std::ios::fixed) << std::setprecision(precision);
        ostr <<"alpha: "<<angles.alpha * toDeg<<", ";
        ostr <<"beta: "<<angles.beta * toDeg<<", ";
        ostr <<"gamma: "<<angles.gamma * toDeg;        
        return ostr.str();
    }
}
