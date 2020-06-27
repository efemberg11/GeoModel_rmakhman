# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloToyDetectorFactory
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloToyDetectorFactory)

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
FILE(GLOB SRCS src/main.cpp src/*.cxx )

# Tell CMake to create the helloworld executable
add_executable( helloToyDetectorFactory ${SRCS} )

# Link all needed libraries
target_link_libraries( helloToyDetectorFactory GeoModelKernel GeoModelDBManager GeoModelWrite )
