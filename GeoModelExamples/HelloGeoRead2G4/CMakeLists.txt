# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloGeoRead2G4
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

#project(HelloGeoRead2G4)

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the needed dependencies, when building individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) 
  find_package( GeoModelCore REQUIRED  )
  find_package( GeoModelIO REQUIRED ) 
endif()

## External dependencies.
find_package( Geant4 REQUIRED )
# include Geant4 headers
include(${Geant4_USE_FILE})

# Populate a CMake variable with the sources
set( SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( hellogeoRead2G4 ${SRCS} )

# Link all needed libraries
target_link_libraries( hellogeoRead2G4 GeoModelDBManager GeoModelRead GeoModelKernel GeoModel2G4  ${Geant4_LIBRARIES})
