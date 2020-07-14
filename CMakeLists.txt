# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# === Preamble ===
cmake_minimum_required(VERSION 3.10)
project( "GeoModel" VERSION 1.0.0 LANGUAGES CXX )
# enable_testing()


# === Project wide setup ===
# Make the 'cmake' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
# Define color codes for CMake messages
include( cmake_colors_defs )
# Set build and C++ default options
include( configure_cpp_options )
# Use the GNU install directory names.
include( GNUInstallDirs )
# Set a default build type
include( BuildType )
# Print Build Info on screen
include( PrintBuildInfo )

# === Externally provided content ===
# Make the 'dependencies' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/dependencies )
include(Dependencies)

# === Main targets built by this project ===

# switches to let users build specific packages on request
option(GEOMODEL_BUILD_EXAMPLES "Enable the build of GeoModelExamples" OFF)
option(GEOMODEL_BUILD_EXAMPLES_W_GEANT4 "Enable the build of GeoModelExamples, including the ones concerning the interface GeoModel->Geant4" OFF)
option(GEOMODEL_BUILD_VISUALIZATION "Enable the build of GeoModelVisualization" OFF)
option(GEOMODEL_BUILD_GEOMODELG4 "Enable the build of GeoModelG4" OFF)
option(GEOMODEL_BUILD_FULLSIMLIGHT "Enable the build of FullSimLight" OFF)

# a list to keep track of the packages we build
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

if(GEOMODEL_BUILD_EXAMPLES)
  add_subdirectory(GeoModelExamples)
  list( APPEND BUILT_PACKAGES "GeoModelExamples")
endif()

if(GEOMODEL_BUILD_FULLSIMLIGHT)
  set(GEOMODEL_BUILD_GEOMODELG4 TRUE) # FullSimLight needs GeoModelG4
  add_subdirectory(FullSimLight)
  list( APPEND BUILT_PACKAGES "FullSimLight")
endif()

if(GEOMODEL_BUILD_GEOMODELG4 OR GEOMODEL_BUILD_EXAMPLES_W_GEANT4)
  add_subdirectory(GeoModelG4)
  list( APPEND BUILT_PACKAGES "GeoModelG4")
endif()


# A function to get a string with comma-separated package names from a list of packages
# NOTE: We could make use of list(JOIN ...) on CMake >= 3.12,
# but Ubuntu 18 pat package installs 3.10 instead
function(getCSVStringFromList inputList outputString)
  message("input: ${inputList}")
  set(tempList "")
  foreach( item ${inputList})
    list(APPEND tempList ${item})
  endforeach()
  string (REPLACE ";" ", " outStr "${tempList}")
  message("string: ${outStr}")
  set( ${outputString} ${outStr} PARENT_SCOPE)
endfunction()


# Let the users know which and how many packages they are building
list(LENGTH BUILT_PACKAGES BUILT_PACKAGES_LENGTH)
#
# list(JOIN BUILT_PACKAGES ", " BUILT_PACKAGES_STR) # list(JOIN) needs /CMake 3.12, which is missing on Ubuntu 18 by default
getCSVStringFromList( "${BUILT_PACKAGES}" BUILT_PACKAGES_STR )
#
message(STATUS "${BoldWhite}-----${ColourReset}")
message( STATUS "${BoldGreen}Building the following ${BUILT_PACKAGES_LENGTH} packages: ${BUILT_PACKAGES_STR}${ColourReset}")
message(STATUS "${BoldWhite}-----${ColourReset}")

# === Things typically only needed if we are the top level project ===
# if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#     add_subdirectory(tests)
#     add_subdirectory(packaging)
# endif()
