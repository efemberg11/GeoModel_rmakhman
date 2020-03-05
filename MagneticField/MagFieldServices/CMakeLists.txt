################################################################################
# Package: MagFieldServices
################################################################################

cmake_minimum_required(VERSION 3.10) 

# Declare the package name:
project( "MagFieldServices" VERSION 1.0.0 LANGUAGES CXX )
#atlas_subdir( MagFieldServices )

# Declare the package's dependencies:
#atlas_depends_on_subdirs( PUBLIC
#                          Control/AthenaBaseComps
#                          MagneticField/MagFieldInterfaces
#                          PRIVATE
#                          Control/StoreGate
#                          Database/AthenaPOOL/AthenaPoolUtilities
#                          GaudiKernel
#                          Tools/PathResolver )

# External dependencies:
find_package( CLHEP )

#find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO pthread )





# Component(s) in the package:
#atlas_add_component( MagFieldServices
#                     src/*.cxx
#                     src/components/*.cxx
#                     INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${CLHEP_INCLUDE_DIRS}
#                     LINK_LIBRARIES ${ROOT_LIBRARIES} ${CLHEP_LIBRARIES} AthenaBaseComps MagFieldInterfaces StoreGateLib SGtests AthenaPoolUtilities GaudiKernel PathResolver )

# Install files from the package:
#atlas_install_headers( MagFieldServices )
#atlas_install_python_modules( python/*.py )

#if( NOT SIMULATIONBASE )
#  atlas_add_test( MagFieldServicesConfig    SCRIPT python -m MagFieldServices.MagFieldServicesConfig
#                  PROPERTIES TIMEOUT 300
#                 POST_EXEC_SCRIPT nopost.sh )
#endif()


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

