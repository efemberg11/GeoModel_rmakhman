
cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "GeoModelKernel" VERSION 1.0.0 LANGUAGES CXX )

# find_package( GeoGenericFunctions REQUIRED )


# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Set up how Eigen should be used.
option( BUILTIN_EIGEN3 "Download a version of Eigen3 for the build" FALSE )
if( BUILTIN_EIGEN3 )
   # Tell the user what's happening.
   message( STATUS "Building Eigen3 as part of the project" )
   # Build/install Eigen3 using ExternalProject_Add(...).
   include( ExternalProject )
   ExternalProject_Add( Eigen3
      PREFIX ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Build
      INSTALL_DIR
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install
      URL "http://cern.ch/lcgpackages/tarFiles/sources/eigen-3.2.9.tar.gz"
      URL_MD5 "de04f424e6b86907ccc9737b5d3048e7"
      CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> )
   ExternalProject_Add_Step( Eigen3 removepc
      COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/share
      COMMENT "Removing the pkgconfig file from Eigen"
      DEPENDEES install )
   install( DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install/
      DESTINATION . USE_SOURCE_PERMISSIONS )
   # Specify the include directory under which the Eigen3 headers will be
   # available.
   set( EIGEN3_INCLUDE_DIR
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install/include/eigen3>
      $<INSTALL_INTERFACE:include/eigen3> )
else()
   # Find an existing installation of Eigen3.
   list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
   find_package( Eigen3 REQUIRED )
endif()

# # Build the GeoGenericFunctions library.
# add_subdirectory( external/GeoGenericFunctions )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelKernel/*.h )

# Create the library.
add_library( GeoModelKernel SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET GeoModelKernel
   PROPERTY PUBLIC_HEADER ${HEADERS} )
target_link_libraries( GeoModelKernel PUBLIC GeoGenericFunctions )
target_include_directories( GeoModelKernel SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR} )
target_include_directories( GeoModelKernel PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoModelKernel" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
if( BUILTIN_EIGEN3 )
   add_dependencies( GeoModelKernel Eigen3 )
endif()

# # Install the library.
# install( TARGETS GeoModelKernel
#    EXPORT GeoModelKernel
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelKernel )

# Install a CMake description of the project/library.
# install( EXPORT GeoModelKernel DESTINATION cmake )


# new test GeoModelCore
install( TARGETS GeoModelKernel EXPORT GeoModelKernel-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelKernel )
#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GeoModelKernel PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
