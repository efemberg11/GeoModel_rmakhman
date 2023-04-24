# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoModel2G4
################################################################################

cmake_minimum_required(VERSION 3.16...3.26)

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

# set the path where the G4 extensions are found (e.g., the LArCustomSolid ATLAS extension)
set(PATHEXTENSIONS ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} )
add_definitions (-DG4EXTENSION_SOLID_DIR=${PATHEXTENSIONS})
message("==> NOTE: G4 solid extensions will be searched in: ${PATHEXTENSIONS}. If you need custom extensions check if this is the right path. If not, set the right path by setting the environmental variable 'G4EXTENSION_SOLID_DIR' before running the application that uses the GeoModel2G4 library (e.g., 'export G4EXTENSION_SOLID_DIR=/usr/local/Cellar/geomodel-extension-atlaslar/5.4.0/lib') " )

# Set target and properties
add_library( GeoModel2G4 SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModel2G4
  PUBLIC  ${GEANT4_LIBRARIES}
  PRIVATE GeoMaterial2G4 GeoModelCore::GeoModelKernel )

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
