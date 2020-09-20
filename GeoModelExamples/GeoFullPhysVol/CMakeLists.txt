# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloToy
# author: Riccardo Maria BIANCHI @ CERN - Aug, 2020
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoFullPhysVolEx)

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS step1_create_store_geo_and_publish_nodes.cpp )
set(SRCS_READ step2_read_geo_and_published_nodes.cpp )

# Tell CMake to create the 'fullphysvolex' executable
add_executable( fullphysvolex ${SRCS} )
add_executable( read_fullphysvolex_and_publishedNodes ${SRCS_READ} )

# Link all needed libraries
target_link_libraries( fullphysvolex GeoModelKernel GeoModelDBManager GeoModelWrite GeoModelRead )
target_link_libraries( read_fullphysvolex_and_publishedNodes GeoModelKernel GeoModelDBManager GeoModelRead )
