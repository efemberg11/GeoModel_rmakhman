################################################################################
# Package: MagFieldServices
################################################################################

cmake_minimum_required(VERSION 3.10) 

# Declare the package name:
project( "MagFieldServices" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( CLHEP )

# Project's Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS MagFieldServices/*.h )

# Set target and properties
add_library( MagFieldServices SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( MagFieldServices PUBLIC ${CLHEP_LIBRARIES} )
target_include_directories( MagFieldServices PUBLIC ${CLHEP_INCLUDE_DIRS}
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

# Set installation of library headers
set_property( TARGET MagFieldServices PROPERTY PUBLIC_HEADER ${HEADERS} )

# new test MagFieldServices
install( TARGETS MagFieldServices EXPORT MagFieldServices-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MagFieldServices )

