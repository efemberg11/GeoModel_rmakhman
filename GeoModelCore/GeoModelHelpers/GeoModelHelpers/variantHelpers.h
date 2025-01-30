// Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration

/*
 * This header file provides helper C++ functions used in GeoModel IO code. 
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: April, 2024
 *
 */


#ifndef GEOMODELHELPERS_VARIANTHELPERS_H
#define GEOMODELHELPERS_VARIANTHELPERS_H

#include "GeoModelKernel/throwExcept.h"

#include <iostream> 
#include <string> 
#include <vector> 
#include <variant> 
#include <type_traits> 


namespace GeoModelHelpers {

    class variantHelper {
    public:
        using VariantType_t = std::variant<int, long, float, double, std::string>;
        static void printStdVectorVariants(const std::vector<VariantType_t>& vec) {
            for (const auto &item : vec)
            {
                if (std::holds_alternative<int>(item))
                    std::cout << std::get<int>(item);
                else if (std::holds_alternative<long>(item))
                    std::cout << std::get<long>(item);
                else if (std::holds_alternative<float>(item))
                    std::cout << std::get<float>(item);
                else if (std::holds_alternative<double>(item))
                    std::cout << std::get<double>(item);
                else if (std::holds_alternative<std::string>(item))
                    std::cout << std::get<std::string>(item);

                std::cout << ", ";
            }
                std::cout << std::endl;
        }

        static std::string getFromVariant_String(const VariantType_t &record, std::string_view logMsg = "") {
            constexpr std::string_view type{"string"};
            std::string ret;
            try {
                ret = std::get<std::string>(record);
            } catch (std::bad_variant_access const &ex){
                THROW_EXCEPTION(ex.what()<<": '"<<logMsg<<"'  is not a '"<<type<<"'! It's a '" 
                                         <<getFromVariant_Type(record)<<"'.");
            }
            return ret;
        }
        static int getFromVariant_Int(const VariantType_t &record, std::string_view logMsg = "") {
            std::string_view type{"int"};
            int ret{0};
            
            try {
                ret = std::get<int>(record);
            }
            catch (std::bad_variant_access const &ex) {
                THROW_EXCEPTION(ex.what()<<": '"<<logMsg<<"'  is not a '"<<type<<"'! It's a '" 
                                         <<getFromVariant_Type(record)<<"'.");
            }
            return ret;
        }
        static double getFromVariant_Double(const VariantType_t &record, std::string_view logMsg = "")  {
            constexpr std::string_view type{"double"};
            double ret{0.};
            try {
                ret = std::get<double>(record);
            }
            catch (std::bad_variant_access const &ex) {
                THROW_EXCEPTION(ex.what()<<": '"<<logMsg<<"'  is not a '"<<type<<"'! It's a '" 
                                         <<getFromVariant_Type(record)<<"'.");
            }
            return ret;
        }
        static std::string getFromVariant_Type(const VariantType_t &record) {
            std::string type;
            if (std::holds_alternative<int>(record)) {
                type = "int";
            }
            else if (std::holds_alternative<long>(record)) {
                type = "long";
            }
            else if (std::holds_alternative<float>(record)) {
                type = "float";
            }
            else if (std::holds_alternative<double>(record)){ 
                type = "double";
            }
            else if (std::holds_alternative<std::string>(record)) {
                type = "string";
            } else {
                type = "UNKOWN";
            }
            return type;
        }
    };
} // end of namespace


#endif

