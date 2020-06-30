# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This module is used to set up XercesC for the project. Either by looking for
# it on the build machine, or by downloading it during the build itself.
#

# Configuration option for how XercesC should be used.
option( GEOMODEL_USE_BUILTIN_XERCESC
   "Download/build a version of XercesC during the build" FALSE )

# Now do what was requested.
if( GEOMODEL_USE_BUILTIN_XERCESC )

   # Tell the user what's happening.
   message( STATUS "Building XercesC as part of the project" )

   # Tell the user what's happening.
   if( COLOR_DEFS )
     message( STATUS "${BoldMagenta}'GEOMODEL_USE_BUILTIN_XERCESC' was set to 'true' ==> Building XercesC as part of the project${ColourReset}" )
   else()
     message( STATUS "'GEOMODEL_USE_BUILTIN_XERCESC' was set to 'true' ==> Building XercesC as part of the project" )
   endif()

   # The include directory and library that will be produced.
   set( XercesC_INCLUDE_DIR
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall/${CMAKE_INSTALL_INCLUDEDIR}" )
   set( XercesC_INCLUDE_DIRS "${XercesC_INCLUDE_DIR}" )
   set( XercesC_LIBRARY
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall/lib/${CMAKE_SHARED_LIBRARY_PREFIX}xerces-c${CMAKE_SHARED_LIBRARY_SUFFIX}" )
   set( XercesC_LIBRARIES "${XercesC_LIBRARY}" )

   # Create the include directory already, otherwise CMake refuses to
   # create the imported target.
   file( MAKE_DIRECTORY "${XercesC_INCLUDE_DIR}" )

   # Build/install Eigen3 using ExternalProject_Add(...).
   include( ExternalProject )
   ExternalProject_Add( XercesC
      PREFIX "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCBuild"
      INSTALL_DIR "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall"
      URL "https://cern.ch/lcgpackages/tarFiles/sources/xerces-c-3.1.3.tar.gz"
      URL_MD5 "70320ab0e3269e47d978a6ca0c0e1e2d"
      CONFIGURE_COMMAND
      ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++${CMAKE_CXX_STANDARD}
      <SOURCE_DIR>/configure --disable-static --prefix=<INSTALL_DIR>
      INSTALL_COMMAND make install
      COMMAND ${CMAKE_COMMAND} -E remove -f
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall/lib/${CMAKE_SHARED_LIBRARY_PREFIX}xerces-c.la"
      COMMAND ${CMAKE_COMMAND} -E remove_directory
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall/lib/pkgconfig"
      BUILD_BYPRODUCTS "${XercesC_INCLUDE_DIR}" "${XercesC_LIBRARY}" )
   install( DIRECTORY
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall/bin"
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/XercesCInstall/lib"
      DESTINATION .
      COMPONENT Runtime
      USE_SOURCE_PERMISSIONS )
   install( DIRECTORY "${XercesC_INCLUDE_DIR}"
      DESTINATION .
      COMPONENT Development
      USE_SOURCE_PERMISSIONS )

   # Set up XercesC's imported target.
   add_library( XercesC::XercesC UNKNOWN IMPORTED )
   set_target_properties( XercesC::XercesC PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${XercesC_INCLUDE_DIR}"
      IMPORTED_LOCATION "${XercesC_LIBRARY}" )

else()

   # Just find an existing installation of XercesC.
   find_package( XercesC REQUIRED )

endif()
