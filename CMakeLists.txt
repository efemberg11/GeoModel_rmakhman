# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelExamples" VERSION 1.2.1 LANGUAGES CXX )
add_subdirectory( HelloGeoWrite )
add_subdirectory( HelloGeoRead )
add_subdirectory( GeoTessellatedSolidExamples )
add_subdirectory( HelloDummyMaterial )
