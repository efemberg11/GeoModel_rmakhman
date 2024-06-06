
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

        // Function to increase the precision of the conversion from double to string -
        // used in the write operation
        static std::string to_string_with_precision(const double a_value, const int n = 16)
        {
            std::ostringstream out;
            out.precision(n);
            out << std::fixed << a_value;
            return out.str();
        }

        static void printStdVectorStrings(std::vector<std::string> vec)
        {
            for (const auto &str : vec)
            {
                std::cout << str << " ";
            }
            std::cout << std::endl;
            return;
        }

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

        // static std::string getEnvVar(std::string const &key)
        // {
        //     char *val = std::getenv(key.c_str());
        //     return val == NULL ? std::string("") : std::string(val);
        // }

        // static void printStdVectorVariants(const std::vector<std::variant<int, long, float, double, std::string>> vec)
        // {
        //     for (const auto &item : vec)
        //     {
        //         if (std::holds_alternative<int>(item))
        //             std::cout << std::get<int>(item); // INT
        //         else if (std::holds_alternative<long>(item))
        //             std::cout << std::get<long>(item);
        //         else if (std::holds_alternative<float>(item))
        //             std::cout << std::get<float>(item);
        //         else if (std::holds_alternative<double>(item))
        //             std::cout << std::get<double>(item);
        //         else if (std::holds_alternative<std::string>(item))
        //             std::cout << std::get<std::string>(item);

        //         std::cout << ", ";
        //     }
        //         std::cout << std::endl;
        // }

        // static std::string getFromVariant_String(const std::variant<int, long, float, double, std::string> &record, std::string_view logMsg = "")
        // {
        //     std::string_view type{"string"};
        //     std::string ret;
        //     try
        //     {
        //         ret = std::get<std::string>(record);
        //     }
        //     catch (std::bad_variant_access const &ex)
        //     {
        //         std::cout << ex.what() << ": '" << logMsg << "'  is not a 'string'!\n";
        //     }
        //     return ret;
        // }
        // static int getFromVariant_Int(const std::variant<int, long, float, double, std::string> &record, std::string_view logMsg = "")
        // {
        //     std::string_view type{"int"};
        //     int ret;
            
        //     try
        //     {
        //         ret = std::get<int>(record);
        //     }
        //     catch (std::bad_variant_access const &ex)
        //     {
        //         std::cout << ex.what() << ": '" << logMsg << "'  is not a '" << type << "'!\n";
        //     }
        //     return ret;
        // }
        // static int getFromVariant_Double(const std::variant<int, long, float, double, std::string> &record, std::string_view logMsg = "")
        // {
        //     std::string_view type{"double"};
        //     double ret;
        //     try
        //     {
        //         ret = std::get<double>(record);
        //     }
        //     catch (std::bad_variant_access const &ex)
        //     {
        //         std::cout << ex.what() << ": '" << logMsg << "'  is not a '" << type << "'!\n";
        //     }
        //     return ret;
        // }
        // static std::string getFromVariant_Type(const std::variant<int, long, float, double, std::string> &record)
        // {
        //     std::string type;
        //     if (std::holds_alternative<int>(record))
        //     {
        //         type = "int";
        //     }
        //     else if (std::holds_alternative<long>(record))
        //     {
        //         type = "long";
        //     }
        //     else if (std::holds_alternative<float>(record))
        //     {
        //         type = "float";
        //     }
        //     else if (std::holds_alternative<double>(record))
        //     {
        //         type = "double";
        //     }
        //     else if (std::holds_alternative<std::string>(record))
        //     {
        //         type = "string";
        //     } else {
        //         type = "UNKOWN";
        //     }
        //     std::cout << "Variant is of type '" << type << "'" << std::endl;
        //     return type;
        // }
    };
} // namespace GeoModelIO

#endif
