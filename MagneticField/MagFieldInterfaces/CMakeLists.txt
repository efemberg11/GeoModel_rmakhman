################################################################################
# Package: MagFieldInterfaces
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "MagFieldInterfaces" VERSION 1.0.0)



# Declare the package's dependencies:

# External dependencies:
#find_package( Eigen3 REQUIRED )



# Project's Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Use the GNU install directory names.
#include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR


# Find the header and source files.
#file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS MagFieldInterfaces/*.h )

# Set target and properties

add_library( MagFieldInterfaces INTERFACE)

target_link_libraries( MagFieldInterfaces INTERFACE)

target_include_directories( MagFieldInterfaces INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

# Set installation of library headers
set_property( TARGET MagFieldInterfaces PROPERTY PUBLIC_HEADER ${HEADERS} )

# new test MagFieldServices
install( TARGETS MagFieldInterfaces EXPORT MagFieldInterfaces-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MagFieldInterfaces )
