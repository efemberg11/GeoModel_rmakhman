################################################################################
# Package: GeoMaterial2G4
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoMaterial2G4" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( Geant4 REQUIRED )
find_package( GeoModelCore REQUIRED )

# Project's Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoMaterial2G4/*.h )

# include Geant4 headers
include(${Geant4_USE_FILE})

# Set target and properties
add_library( GeoMaterial2G4 SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoMaterial2G4 PUBLIC ${Geant4_LIBRARIES} GeoModelKernel GeoModelUtilities )
target_include_directories( GeoMaterial2G4 SYSTEM PUBLIC ${GeoModelCore_INCLUDE_DIRS} )
target_include_directories( GeoMaterial2G4 PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

# Set installation of library headers
set_property( TARGET GeoMaterial2G4 PROPERTY PUBLIC_HEADER ${HEADERS} )

# # Install the library and set export target
# install( TARGETS GeoMaterial2G4
#   EXPORT GeoMaterial2G4Config
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoMaterial2G4
# )
#
# # Install a CMake description of the project/library.
# install( EXPORT GeoMaterial2G4Config DESTINATION cmake )

# new test GeoModelG4
install( TARGETS GeoMaterial2G4 EXPORT GeoMaterial2G4-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoMaterial2G4 )
