# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This module is used to set up Coin3D for the project. Either by looking for
# it on the build machine, or by downloading it during the build itself.
#

# Make sure that this file is only included once.
get_property( _coinSetUp GLOBAL PROPERTY GEOMODEL_COIN3D_SET_UP SET )
if( _coinSetUp )
   unset( _coinSetUp )
   return()
endif()
set_property( GLOBAL PROPERTY GEOMODEL_COIN3D_SET_UP TRUE )

# Configuration option for how XercesC should be used.
option( GEOMODEL_USE_BUILTIN_COIN3D
   "Download/build a version of Coin3D during the build" FALSE )

# Now do what was requested.
if( GEOMODEL_USE_BUILTIN_COIN3D )

   # The include directory and library that will be produced.
   set( COIN3D_INCLUDE_DIR
       "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DInstall/${CMAKE_INSTALL_INCLUDEDIR}" )
   set( COIN3D_INCLUDE_DIRS "${COIN3D_INCLUDE_DIR}" )
   set( COIN3D_LIBRARY
       "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DInstall/${CMAKE_INSTALL_LIBDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Coin${CMAKE_SHARED_LIBRARY_SUFFIX}" )
   set( COIN3D_LIBRARIES "${COIN3D_LIBRARY}" )

   # Create the include directory already, otherwise CMake refuses to
   # create the imported target.
   file( MAKE_DIRECTORY "${COIN3D_INCLUDE_DIR}" )

   # Build/install Coin3D using ExternalProject_Add(...).
   include( ExternalProject )
   ExternalProject_Add( Coin3DBuiltIn
      PREFIX "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DBuild"
      INSTALL_DIR "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DInstall"
      URL "https://github.com/coin3d/coin/releases/download/Coin-4.0.0/coin-4.0.0-src.tar.gz"
      URL_MD5 "2377d11b38c8eddd92d8bb240f5bf4ee"
      CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DInstall
      -DCMAKE_INSTALL_BINDIR:PATH=${CMAKE_INSTALL_BINDIR}
      -DCMAKE_INSTALL_LIBDIR:PATH=${CMAKE_INSTALL_LIBDIR}
      -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCOIN_BUILD_TESTS:BOOL=FALSE
      -DCOIN_BUILD_DOCUMENTATION:BOOL=FALSE
      BUILD_BYPRODUCTS "${COIN3D_INCLUDE_DIR}" "${COIN3D_LIBRARY}" )
   install( DIRECTORY
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DInstall"
      DESTINATION .
      COMPONENT Runtime
      USE_SOURCE_PERMISSIONS )

   # Set up Coin3D's imported target.
   add_library( Coin::Coin UNKNOWN IMPORTED )
   set_target_properties( Coin::Coin PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${COIN3D_INCLUDE_DIR}"
      IMPORTED_LOCATION "${COIN3D_LIBRARY}" )

   # The include directory and library that will be produced.
   set( SoQt_INCLUDE_DIR
       "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SoQtInstall/${CMAKE_INSTALL_INCLUDEDIR}" )
   set( SoQt_INCLUDE_DIRS "${SoQt_INCLUDE_DIR}" )
   set( SoQt_LIBRARY
       "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SoQtInstall/${CMAKE_INSTALL_LIBDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}SoQt${CMAKE_SHARED_LIBRARY_SUFFIX}" )
   set( SoQt_LIBRARIES "${SoQt_LIBRARY}" )

   # Create the include directory already, otherwise CMake refuses to
   # create the imported target.
   file( MAKE_DIRECTORY "${SoQt_INCLUDE_DIR}" )

   # Build/install SoQt using ExternalProject_Add(...).
   ExternalProject_Add( SoQtBuiltIn
      PREFIX "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SoQtBuild"
      INSTALL_DIR "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SoQtInstall"
      URL "https://github.com/coin3d/soqt/releases/download/SoQt-1.6.0/soqt-1.6.0-src.tar.gz"
      URL_MD5 "724996aedad2a33760dc36f08ceeda22"
      CMAKE_CACHE_ARGS
      -DCMAKE_PREFIX_PATH:PATH=${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Coin3DInstall
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SoQtInstall
      -DCMAKE_INSTALL_BINDIR:PATH=${CMAKE_INSTALL_BINDIR}
      -DCMAKE_INSTALL_LIBDIR:PATH=${CMAKE_INSTALL_LIBDIR}
      -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DSOQT_BUILD_DOCUMENTATION:BOOL=FALSE
      BUILD_BYPRODUCTS "${SoQt_INCLUDE_DIR}" "${SoQt_LIBRARY}"
      DEPENDS Coin3DBuiltIn )
   install( DIRECTORY
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SoQtInstall"
      DESTINATION .
      COMPONENT Runtime
      USE_SOURCE_PERMISSIONS )

   # Set up SoQt's imported target.
   add_library( SoQt::SoQt UNKNOWN IMPORTED )
   set_target_properties( SoQt::SoQt PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${SoQt_INCLUDE_DIR}"
      IMPORTED_LOCATION "${SoQt_LIBRARY}" )

else()

   # Just find an existing installation of Coin3D.
   find_package( Coin REQUIRED )
   find_package( SoQt REQUIRED )

endif()
