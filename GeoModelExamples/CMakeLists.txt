# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoModelExamples
# author: Riccardo Maria BIANCHI @ CERN - Nov, 2018
################################################################################

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelExamples" VERSION 4.1.0 LANGUAGES CXX )

# Getting-started examples
add_subdirectory( KitchenSinkPlugin )

add_subdirectory( HelloGeo )
add_subdirectory( HelloGeoWrite )
#add_subdirectory( HelloGeoRead )
#add_subdirectory( HelloDummyMaterial )
#add_subdirectory( HelloToy )
#add_subdirectory( HelloToyDetectorFactory )

# GeoModel reference examples
#add_subdirectory( GeoActions )
#add_subdirectory( GeoShapeExamples )
#add_subdirectory( GeoShiftUnion )
#add_subdirectory( GeoTessellatedSolidExamples )
#add_subdirectory( GeoFullPhysVol )

# GeoModel <--> Geant4 examples are built on explicit request only
if(GEOMODEL_BUILD_EXAMPLES_W_GEANT4)
  add_subdirectory( HelloGeoRead2G4 )
  # list( APPEND BUILT_PACKAGES "HelloGeoRead2G4")
endif()
