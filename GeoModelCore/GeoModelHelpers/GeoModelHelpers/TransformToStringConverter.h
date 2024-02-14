/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_TRANSFORMTOSTRINGCONVERTER_H
#define GeoModelHelpers_TRANSFORMTOSTRINGCONVERTER_H
#include "GeoModelKernel/GeoDefinitions.h"

#include <string>
#include <sstream>
#include <iomanip>
namespace GeoTrf{
    
    template <int N>
    inline std::string toString(const VectorN<N>& vector, int precision = 4) {
        std::stringstream sout;
        sout << std::setiosflags(std::ios::fixed) << std::setprecision(precision);
        sout << "(";
        for (int i = 0; i < N; ++i) {
            sout << vector[i];
            if (i != N - 1) {
                sout << ", ";
            }
        }
        sout << ")";
        return sout.str();
    }

    std::string toString(const Transform3D& trans, bool useCoordAngles = false, int precision = 4);
    std::string toString(const EulerAngles& angles, int precision = 4);
    std::string toString(const CoordEulerAngles& angles, int precision = 4);
}

#endif