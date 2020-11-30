# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoShapeExamples
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoShapeExamples)


# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package( GeoModelCore REQUIRED 4.1.0 ) #TODO: the version should be se in the root folder 
find_package( GeoModelIO REQUIRED 4.1.0 ) #TODO: the version should bese in the root folder



# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( geoShapeExamples ${SRCS} )

# Link all needed libraries
target_link_libraries( geoShapeExamples GeoModelCore::GeoModelKernel GeoModelIO::GeoModelWrite )
