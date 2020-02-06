################################################################################
# Package: GeoModel2G4
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModel2G4" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( Geant4 REQUIRED )
find_package( CLHEP )
# GeoModel dependencies
find_package( GeoModelCore REQUIRED )
find_package (GeoModelIO REQUIRED)

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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModel2G4/*.h )

# include Geant4 headers
include(${Geant4_USE_FILE})

# Set target and properties
add_library( GeoModel2G4 SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModel2G4
  PUBLIC  ${GEANT4_LIBRARIES}
  PRIVATE ${CLHEP_LIBRARIES} GeoMaterial2G4 GeoModelIO::GeoSpecialShapes)
target_include_directories( GeoModel2G4 SYSTEM PUBLIC ${GEANT4_INCLUDE_DIRS} PRIVATE ${CLHEP_INCLUDE_DIRS} ${GeoModelCore_INCLUDE_DIRS} )
target_include_directories( GeoModel2G4 PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

# Set installation of library headers
set_property( TARGET GeoModel2G4 PROPERTY PUBLIC_HEADER ${HEADERS} )

# # Install the library.
# install( TARGETS GeoModel2G4
#    EXPORT GeoModel2G4
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModel2G4 )
#
# install( TARGETS GeoModel2G4
#    EXPORT GeoModel2G4Config
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModel2G4 )
#
# # Install a CMake description of the project/library.
# install( EXPORT GeoModel2G4Config DESTINATION cmake )

# new test GeoModelG4
install( TARGETS GeoModel2G4 EXPORT GeoModel2G4-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModel2G4 )
