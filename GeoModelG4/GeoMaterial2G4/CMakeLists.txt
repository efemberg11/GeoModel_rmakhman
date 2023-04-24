# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoMaterial2G4
################################################################################
cmake_minimum_required(VERSION 3.16...3.26)
project( "GeoMaterial2G4" VERSION ${GeoModel_VERSION} LANGUAGES CXX )

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoMaterial2G4/*.h )

# include Geant4 headers
include(${Geant4_USE_FILE})

# Set target and properties
add_library( GeoMaterial2G4 SHARED ${HEADERS} ${SOURCES} )

# Check if we are building FullSimLight individually,
# or as a part of the main GeoModel.
# In the first case, we link against the imported targets, which are taken
# from the base GeoModel packages already installed on the system.
# This is used when building distribution packages.
if ( GeoModelG4_INDIVIDUAL_BUILD ) # if built individually
    target_link_libraries( GeoMaterial2G4 PUBLIC GeoModelCore::GeoModelKernel )
else()
    target_link_libraries( GeoMaterial2G4 PUBLIC GeoModelKernel )
endif()
target_link_libraries( GeoMaterial2G4 PUBLIC ${Geant4_LIBRARIES} )

target_include_directories( GeoMaterial2G4 PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

# Set installation of library headers
set_property( TARGET GeoMaterial2G4 PROPERTY PUBLIC_HEADER ${HEADERS} )

set_target_properties( GeoMaterial2G4 PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# new test GeoModelG4
install( TARGETS GeoMaterial2G4 EXPORT GeoMaterial2G4-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoMaterial2G4 )
