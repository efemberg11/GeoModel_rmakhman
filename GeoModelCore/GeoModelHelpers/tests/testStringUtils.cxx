/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/StringUtils.h"
#include "GeoModelHelpers/throwExcept.h"
#include <iostream>
#include <stdlib.h>
using namespace GeoStrUtils;

namespace {
    std::string getEnvVar(const std::string& varName){
       return std::getenv(varName.c_str()) ? std::string{std::getenv(varName.c_str())} : std::string{};
    }
}

int main() {
   if (resolveEnviromentVariables("${PWD}") != getEnvVar("PWD")) {
      THROW_EXCEPTION("PWD has been resolved differently "<<resolveEnviromentVariables("${PWD}")
                      <<" vs. "<<getEnvVar("PWD"));
   }

   if (resolveEnviromentVariables("${PWD}/Kuchen") != getEnvVar("PWD")+"/Kuchen") {
      THROW_EXCEPTION("PWD/Kuchen has been resolved differently "<<resolveEnviromentVariables("${PWD}/Kuchen")
                      <<" vs. "<<getEnvVar("PWD")<<"/Kuchen");
   }
 
    return EXIT_SUCCESS;
}