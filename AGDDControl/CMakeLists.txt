cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "AGDDControl" VERSION 1.0.0 LANGUAGES CXX )

find_package( GeoModelCore REQUIRED )
find_package( Eigen3 REQUIRED )
find_package( CLHEP REQUIRED )
find_package( GeoModelTools REQUIRED )


# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS AGDDControl/*.h )

# Create the library.
add_library( AGDDControl SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET AGDDControl
   PROPERTY PUBLIC_HEADER ${HEADERS} )
target_link_libraries( AGDDControl PUBLIC  AGDDKernel AGDDModel AGDDHandlers GeoModelCore::GeoModelKernel CLHEP::CLHEP )
target_include_directories( AGDDControl SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR} )
target_include_directories( AGDDControl PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "AGDDControl" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )

# # Install the library.
# install( TARGETS AGDDControl
#    EXPORT AGDDControl
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TestLib )

# Install a CMake description of the project/library.
# install( EXPORT AGDDControl DESTINATION cmake )


# new test GeoModelCore
install( TARGETS AGDDControl EXPORT AGDDControl-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/AGDDControl )
#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(AGDDControl PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})

