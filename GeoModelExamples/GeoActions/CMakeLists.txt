# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoActions
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoActions)

# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package( GeoModelCore REQUIRED 4.1.0 ) #TODO: the version should be se in the root folder 

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS main.cpp MyVolAction.cxx MyVolActionFilter.cxx )

# Tell CMake to create the helloworld executable
add_executable( geoActions ${SRCS} )

# Link all needed libraries
target_link_libraries( geoActions GeoModelCore::GeoModelKernel )
