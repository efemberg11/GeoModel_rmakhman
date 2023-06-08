# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloToy
# author: Riccardo Maria BIANCHI @ CERN - Aug, 2020
################################################################################

cmake_minimum_required(VERSION 3.16...3.26)

#project(HelloToy)

# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Find the needed dependencies, when building individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) 
  find_package( GeoModelCore REQUIRED  ) 
  find_package( GeoModelIO REQUIRED  ) 
endif()

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS step1_create_store_geo_and_publish_nodes.cpp )
set(SRCS_READ step2_read_geo_and_published_nodes.cpp )

# Tell CMake to create the 'helloToy' executable
add_executable( helloToy ${SRCS} )
add_executable( read_helloToy_and_publishedNodes ${SRCS_READ} )

# Link all needed libraries
target_link_libraries( helloToy GeoModelCore::GeoModelKernel GeoModelIO::GeoModelWrite )
target_link_libraries( read_helloToy_and_publishedNodes GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead )
