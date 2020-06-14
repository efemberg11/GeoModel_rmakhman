################################################################################
# Package: MagFieldServices
################################################################################

cmake_minimum_required(VERSION 3.1) 

# Declare the package name:
project( "MagFieldServices" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( Eigen3 REQUIRED )

if( Eigen3_FOUND )
 message(STATUS "Eigen3 found in: ${EIGEN3_INCLUDE_DIR}")
else()
 message(STATUS "Eigen3 not found")
endif()
#----------------------------------------------------------------------------
# Find Geant4 package, batch mode only executable (i.e. no need ui and vis).
#
find_package(Geant4 REQUIRED)
#----------------------------------------------------------------------------

# Project's Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# ROOT dependency removed
# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#include(${ROOT_USE_FILE})

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS MagFieldServices/*.h )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
set(OUTPUT bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT})

# Set target and properties
add_library( MagFieldServices SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( MagFieldServices PUBLIC ${Geant4_LIBRARIES} MagFieldInterfaces)
target_include_directories( MagFieldServices PUBLIC ${EIGEN3_INCLUDE_DIR}
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

#ROOT_GENERATE_DICTIONARY(G__MagFieldServices ${headers} LINKDEF SolenoidalBFieldLinkDef.h)


# Set installation of library headers
set_property( TARGET MagFieldServices PROPERTY PUBLIC_HEADER ${HEADERS} )


set_target_properties( MagFieldServices PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )



# new test MagFieldServices
install( TARGETS MagFieldServices EXPORT MagFieldServices-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MagFieldServices )

