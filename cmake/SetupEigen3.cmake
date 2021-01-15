# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This module is used to set up Eigen3 for the project. Either by looking for
# it on the build machine, or by downloading it during the build itself.
#

# Make sure that this file is only included once.
get_property( _eigenSetUp GLOBAL PROPERTY GEOMODEL_EIGEN_SET_UP SET )
if( _eigenSetUp )
   unset( _eigenSetUp )
   return()
endif()
set_property( GLOBAL PROPERTY GEOMODEL_EIGEN_SET_UP TRUE )

# Configuration option for how Eigen should be used.
option( GEOMODEL_USE_BUILTIN_EIGEN3
   "Download a version of Eigen3 during the build" FALSE )

# Now do what was requested.
if( GEOMODEL_USE_BUILTIN_EIGEN3 )

   # Tell the user what's happening.
   message( STATUS "Building Eigen3 as part of the project" )

   # Helper variables for the Eigen build.
   set( Eigen3_INCLUDE_DIR
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install/include/eigen3" )
   set( Eigen3_VERSION "3.2.9" )

   # Create the include directory already, otherwise CMake refuses to
   # create the imported target.
   file( MAKE_DIRECTORY "${Eigen3_INCLUDE_DIR}" )

   # Build/install Eigen3 using ExternalProject_Add(...).
   include( ExternalProject )
   ExternalProject_Add( Eigen3
      PREFIX ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Build
      INSTALL_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install
      URL "http://cern.ch/lcgpackages/tarFiles/sources/eigen-${Eigen3_VERSION}.tar.gz"
      URL_MD5 "de04f424e6b86907ccc9737b5d3048e7"
      CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      BUILD_BYPRODUCTS "${Eigen3_INCLUDE_DIR}" )
   ExternalProject_Add_Step( Eigen3 removepc
      COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/share
      COMMENT "Removing the pkgconfig file from Eigen"
      DEPENDEES install )
   install( DIRECTORY
      ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install/
      DESTINATION .
      COMPONENT Development
      USE_SOURCE_PERMISSIONS )

   # Set up an imported target that mimicks the one created by the exported
   # CMake configuration of an Eigen installation.
   add_library( Eigen3::Eigen INTERFACE IMPORTED )
   set_target_properties( Eigen3::Eigen PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${Eigen3_INCLUDE_DIR}" )

else()

   # Just find an existing installation of Eigen3.
   find_package( Eigen3 REQUIRED )

endif()
