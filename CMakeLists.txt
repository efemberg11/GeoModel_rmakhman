# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# === Preamble ===
cmake_minimum_required(VERSION 3.14)

# === Project's settings ===
# Make the 'cmake' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
include( GeoModelATLAS-version )
project( "GeoModelATLAS" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )

# === Project wide setup ===
# Define color codes for CMake messages
include( cmake_colors_defs )
# Use the GNU install directory names.
include( GNUInstallDirs )
# Set a default build type
include( BuildType )
# Print Build Info on screen
include( PrintBuildInfo )
# Set default build and C++ options
include( configure_cpp_options )

# === Externally provided content ===

# By default prefer not to use frameworks on macOS.
# But use it at a "LAST" resource, if no other options worked.
# For details on the behaviour of this cache variable, see:
# - https://cmake.org/cmake/help/v3.0/command/find_file.html
set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
   "Framework finding behaviour on macOS" )

# --- Dependencies --- 
# Set up how the project handle some of its dependenices. Either by picking them
# up from the environment, or building them itself.
include( SetupEigen3 )
include( SetupXercesC )
include( SetupJSON )
# GeoModel dependencies
find_package( GeoModelCore )


# === Main targets built by this project ===

# switches to let users build specific packages on request
option( GEOMODELATLAS_BUILD_DATAMANAGERS "Enable the build of GeoModelDataManagers" OFF )
option( GEOMODELATLAS_BUILD_AGDD "Enable the build of AGDD" OFF )
option( GEOMODELATLAS_BUILD_MUONDD "Enable the build of MuonDD" OFF )
option( GEOMODELATLAS_BUILD_GEOMODELXML "Enable the build of GeoModelXML" OFF )

# a list to keep track of the packages we build
set(BUILT_PACKAGES "")

if( GEOMODELATLAS_BUILD_MUONDD )
    set ( GEOMODELATLAS_BUILD_AGDD TRUE )
    add_subdirectory(MuonDD)
    list( APPEND BUILT_PACKAGES "MuonDD" )
endif()

if( GEOMODELATLAS_BUILD_AGDD )
    set ( GEOMODELATLAS_BUILD_DATAMANAGERS TRUE )
    add_subdirectory(AGDD)
    list( APPEND BUILT_PACKAGES "AGDD" )
endif()

if( GEOMODELATLAS_BUILD_DATAMANAGERS )
    add_subdirectory(GeoModelDataManagers)
    list( APPEND BUILT_PACKAGES "GeoModelDataManagers" )
endif()

if( GEOMODELATLAS_BUILD_GEOMODELXML )
    add_subdirectory(GeoModelXML)
    list( APPEND BUILT_PACKAGES "GeoModelXML" )
endif()


# A function to get a string with comma-separated package names from a list of packages
# NOTE: We could make use of list(JOIN ...) on CMake >= 3.12,
# but on Ubuntu 18 the apt package installs 3.10 instead
function(getCSVStringFromList inputList outputString)
  #message("input: ${inputList}") # for debug
  set(tempList "")
  foreach( item ${inputList})
    list(APPEND tempList ${item})
  endforeach()
  string (REPLACE ";" ", " outStr "${tempList}")
  #message("string: ${outStr}") # for debug
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
