# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This module is used to set up "nlohmann_json" for the project. Either by
# looking for it on the build machine, or by downloading it during the build
# itself.
#

# Make sure that this file is only included once.
include_guard(GLOBAL)

# Configuration option for how "nlohmann_json" should be used.
option(GEOMODEL_USE_BUILTIN_JSON "Download and compile a version of nlohmann_json during the build" OFF)

# Now do what was requested.
if(GEOMODEL_USE_BUILTIN_JSON)
   # Tell the user what's happening.
   message( STATUS "${BoldMagenta}'GEOMODEL_USE_BUILTIN_JSON' was set to 'true' ==> Building nlohmann_json as part of the project${ColourReset}" )

   # The include directory and library that will be produced.
   set(nlohmann_json_INCLUDE_DIR "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JSONInstall/${CMAKE_INSTALL_INCLUDEDIR}" )
   set(nlohmann_json_INCLUDE_DIRS "${nlohmann_json_INCLUDE_DIR}" )
   set(nlohmann_json_VERSION "3.6.1" )
   set(nlohmann_json_FOUND TRUE )
   message(STATUS "Installing the built-in 'nlohmann_json' in: ${nlohmann_json_INCLUDE_DIR}")

   # Create the include directory already, otherwise CMake refuses to
   # create the imported target.
   file(MAKE_DIRECTORY "${nlohmann_json_INCLUDE_DIR}")

   # Build/install nlohmann_json using ExternalProject_Add(...).
   include( ExternalProject )
   ExternalProject_Add(JSONExt
     PREFIX ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JSONBuild
     INSTALL_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JSONInstall
     URL "https://cern.ch/lcgpackages/tarFiles/sources/json-${nlohmann_json_VERSION}.tar.gz"
     URL_MD5 "c53592d55e7fec787cf0a406d36098a3"
     CMAKE_CACHE_ARGS
       -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
       -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
       -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
       -DJSON_BuildTests:BOOL=OFF
       -DJSON_MultipleHeaders:BOOL=ON
     BUILD_BYPRODUCTS "${nlohmann_json_INCLUDE_DIR}" )
   install(DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JSONInstall/
     DESTINATION .
     COMPONENT Development
     USE_SOURCE_PERMISSIONS)

   # Set up nlohmann_json's imported target.
   add_library(nlohmann_json::nlohmann_json INTERFACE IMPORTED)
   set_property(TARGET nlohmann_json::nlohmann_json PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${nlohmann_json_INCLUDE_DIR}")

   # Auto handle dependency
   add_dependencies(nlohmann_json::nlohmann_json JSONExt)

else()
  # Find an existing installation of nlohmann_json.
  find_package(nlohmann_json REQUIRED)
endif()
