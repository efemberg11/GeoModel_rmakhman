# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoModel2G4
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModel2G4" VERSION ${GeoModel_VERSION}  LANGUAGES CXX )

##########################################################
# NOTE! The original package also needs this Athena stuff:
#
# DetectorDescription/GeoModel/GeoModelInterfaces
# DetectorDescription/GeoModel/GeoSpecialShapes
# DetectorDescription/GeoPrimitives
# Simulation/G4Atlas/G4AtlasInterfaces
# Simulation/G4Atlas/G4AtlasTools
# Simulation/G4Sim/SimHelpers

# Project's Settings

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModel2G4/*.h )

# include Geant4 headers
include(${Geant4_USE_FILE})
add_definitions (-DG4EXTENSION_SOLID_DIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} )

# Set target and properties
add_library( GeoModel2G4 SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModel2G4
  PUBLIC  ${GEANT4_LIBRARIES}
  PRIVATE GeoMaterial2G4 )
# Check if we are building FullSimLight individually,
# or as a part of the main GeoModel.
# In the first case, we link against the imported targets, which are taken
# from the base GeoModel packages already installed on the system.^[OB
# This is used when building distribution packages.
if ( GeoModelG4_INDIVIDUAL_BUILD ) # if built individually
    target_link_libraries( GeoModel2G4 PRIVATE GeoModelCore::GeoModelKernel )
else()
    target_link_libraries( GeoModel2G4 PRIVATE GeoModelKernel )
endif()

target_include_directories( GeoModel2G4 SYSTEM PUBLIC ${GEANT4_INCLUDE_DIRS} )
target_include_directories( GeoModel2G4 PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

# Set installation of library headers
set_property( TARGET GeoModel2G4 PROPERTY PUBLIC_HEADER ${HEADERS} )

# Set the library SONAME
set_target_properties( GeoModel2G4 PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
# Install the library.
install( TARGETS GeoModel2G4 EXPORT GeoModel2G4-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModel2G4 )
