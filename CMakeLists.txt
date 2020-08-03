# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "DualUsePlugins" VERSION 2.0.1 LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "Require the specified C++ standard for the build" )

# Make the module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Project's dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( GeoModelDataManagers REQUIRED )
find_package( XercesC REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Set sub-packages to build.
add_subdirectory( ToyGeometryPlugin )
add_subdirectory( AccordionPlugin )
add_subdirectory( HGTDPlugin )
add_subdirectory( Test )
add_subdirectory( LArPlugin )

