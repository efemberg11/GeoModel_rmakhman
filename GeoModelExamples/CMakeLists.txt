# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelExamples" VERSION 1.2.1 LANGUAGES CXX )

# Getting-started examples
add_subdirectory( HelloGeo )
add_subdirectory( HelloGeoWrite )
add_subdirectory( HelloGeoRead )
add_subdirectory( HelloDummyMaterial )
add_subdirectory( HelloToyDetectorFactory )

# GeoModel reference examples
add_subdirectory( GeoActions )
add_subdirectory( GeoShapeExamples )
add_subdirectory( GeoShiftUnion )
add_subdirectory( GeoTessellatedSolidExamples )

# GeoModel <--> Geant4 examples
# add_subdirectory( HelloGeoRead2G4 )

# Make the module directory visible to CMake.
# list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Print Build Info on screen
include(cmake/PrintBuildInfo.cmake)
