/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/StringUtils.h"
#include "GeoModelKernel/throwExcept.h"
#include <cstdlib>
#include <iostream>
using namespace GeoStrUtils;


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