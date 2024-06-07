
// Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

/*
 * This header file provides helper C++ functions used in GeoModel IO code. 
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Apr, 2024
 *
 */


#ifndef GMCPPHelper_H
#define GMCPPHelper_H


// C++ includes
#include <cstdlib>  // EXIT_FAILURE
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <variant> 
#include <type_traits> 

namespace GeoModelIO {

    class CppHelper
    {
    public:
        // dummy constructor
        CppHelper(){};

        // FIXME: should go to an utility class
        static std::string joinVectorStrings(std::vector<std::string> vec,
                                             std::string sep = "")
        {
            std::string s;
            unsigned int ii = 0;
            for (const auto &piece : vec)
            {
                ++ii;
                if (ii == vec.size())
                {
                    s += (piece);
                }
                else
                {
                    s += (piece + sep);
                }
            }
            return s;
        }

    };
} // namespace GeoModelIO

#endif
