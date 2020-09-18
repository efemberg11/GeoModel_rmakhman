/*
Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOUTILFUNCTIONS_H
#define GEOMODELKERNEL_GEOUTILFUNCTIONS_H
 
#include "GeoModelKernel/GeoDefinitions.h"


class GeoUtilFunctions {

    public: 

        static void printTrf(GeoTrf::Transform3D t) {
            std::cout << "transformation: " << std::endl;
            std::cout << "[[" << t(0, 0) << " , ";
            std::cout <<         t(0, 1) << " , ";
            std::cout <<         t(0, 2) << " , ";
            std::cout <<         t(0, 3) << " ]";
            std::cout << "["  << t(1, 0) << " , ";
            std::cout <<         t(1, 1) << " , ";
            std::cout <<         t(1, 2) << " , ";
            std::cout <<         t(1, 3) << " ]";
            std::cout << "["  << t(2, 0) << " , ";
            std::cout <<         t(2, 1) << " , ";
            std::cout <<         t(2, 2) << " , ";
            std::cout <<         t(2, 3) << " ]";
            std::cout << "["  << t(3, 0) << " , ";
            std::cout <<         t(3, 1) << " , ";
            std::cout <<         t(3, 2) << " , ";
            std::cout <<         t(3, 3) << " ]]" << std::endl;
        }

};

#endif
