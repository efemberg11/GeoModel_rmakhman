# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloDummyMaterial
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2019
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloDummyMaterial)

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( helloDummyMaterial ${SRCS} )

# Link all needed libraries
target_link_libraries( helloDummyMaterial GeoModelKernel GeoModelDBManager GeoModelWrite )
