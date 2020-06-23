# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# === Preamble ===
cmake_minimum_required(VERSION 3.1)
project( "GeoModel" VERSION 1.0.0 LANGUAGES CXX )
# enable_testing()


# === Project wide setup ===
# Make the module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
# Set build and C++ default options
include ( configure_cpp_options )
# Use the GNU install directory names.
include( GNUInstallDirs )
# Print Build Info on screen
include( PrintBuildInfo )

# === Externally provided content ===
add_subdirectory(dependencies)

# === Main targets built by this project ===
option(GEOMODEL_BUILD_VISUALIZATION "Enable building GeoModelVisualization" OFF)

set(BUILT_PACKAGES "")

add_subdirectory(GeoModelCore)
list( APPEND BUILT_PACKAGES "GeoModelCore")
add_subdirectory(GeoModelIO)
list( APPEND BUILT_PACKAGES "GeoModelIO")
add_subdirectory(GeoModelTools)
list( APPEND BUILT_PACKAGES "GeoModelTools")
if(GEOMODEL_BUILD_VISUALIZATION)
  add_subdirectory(GeoModelVisualization)
  list( APPEND BUILT_PACKAGES "GeoModelVisualization")
endif()

message(STATUS "-----")
list(LENGTH BUILT_PACKAGES BUILT_PACKAGES_LENGTH)
list(JOIN BUILT_PACKAGES ", " BUILT_PACKAGES_STR)
message( STATUS "Building the following ${BUILT_PACKAGES_LENGTH} packages: ${BUILT_PACKAGES_STR}")
message(STATUS "-----")


# === Things typically only needed if we are the top level project ===
# if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#     add_subdirectory(tests)
#     add_subdirectory(packaging)
# endif()
