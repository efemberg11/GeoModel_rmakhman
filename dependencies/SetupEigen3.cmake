# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This module is used to set up Eigen3 for the project. Either by looking for
# it on the build machine, or by downloading it during the build itself.
#

# Configuration option for how Eigen should be used.
option( GEOMODEL_USE_BUILTIN_EIGEN3
   "Download a version of Eigen3 during the build" FALSE )

# Now do what was requested.
if( GEOMODEL_USE_BUILTIN_EIGEN3 )

   # Tell the user what's happening.
   message( STATUS "Building Eigen3 as part of the project" )

   # Build/install Eigen3 using ExternalProject_Add(...).
   include( ExternalProject )
   ExternalProject_Add( Eigen3
      PREFIX ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Build
      INSTALL_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install
      URL "http://cern.ch/lcgpackages/tarFiles/sources/eigen-3.2.9.tar.gz"
      URL_MD5 "de04f424e6b86907ccc9737b5d3048e7"
      CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> )
   ExternalProject_Add_Step( Eigen3 removepc
      COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/share
      COMMENT "Removing the pkgconfig file from Eigen"
      DEPENDEES install )
   install( DIRECTORY
      ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install/
      DESTINATION .
      COMPONENT Development
      USE_SOURCE_PERMISSIONS )

   # Specify the include directory under which the Eigen3 headers will be
   # available.
   set( EIGEN3_INCLUDE_DIR
      $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Eigen3Install/include/eigen3>
      $<INSTALL_INTERFACE:include/eigen3> )

else()

   # Just find an existing installation of Eigen3.
   find_package( Eigen3 REQUIRED )

endif()
