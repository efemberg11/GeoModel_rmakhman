# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(HelloGeo)

# Find the needed dependencies, when building individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) # when building individually
  find_package( GeoModelCore REQUIRED 4.1.0 ) #TODO: the version should be se in the root folder
endif()

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( hellogeo ${SRCS} )

# Link all needed libraries
target_link_libraries( hellogeo GeoModelCore::GeoModelKernel)
