# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelIO" VERSION 2.1.0 LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "Require the specified C++ standard for the build" )

# Make the module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Project's dependencies.
find_package( GeoModelCore 3.1.0 REQUIRED )
find_package( Qt5 COMPONENTS Core Sql REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )


# Set global installation RPATH settings
# Docs: https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling#always-full-rpath
# -- Set macOS settings
if( APPLE )
  # With these settings you are able to run the programs directly from the build tree.
  # But the RPATH is cleared when installing the executable.
  # So we have to configure the RPATH handling farther, after this instruction.
  SET(CMAKE_MACOSX_RPATH 1)
endif()
# -- Set settings for all platforms
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# set the RPATH used for installed libraries
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
# add the automatically determined parts of the RPATH
# which points to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
     message("Setting RPATH for non-system install directory...")
     SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")


# Set sub-packages to build.
add_subdirectory( GeoModelErrorHandler )
add_subdirectory( GeoModelDBManager )
add_subdirectory( TFPersistification )
add_subdirectory( GeoModelRead )
add_subdirectory( GeoModelWrite )

# Create and install the version description of the project.
include( CMakePackageConfigHelpers )
write_basic_config_version_file(
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion )
install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Create and install the description of the libraries.
install( EXPORT ${PROJECT_NAME}-export
   FILE ${PROJECT_NAME}Targets.cmake
   COMPONENT Development
   NAMESPACE "${PROJECT_NAME}::"
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Install the hand-written project configuration.
configure_file( ${CMAKE_SOURCE_DIR}/cmake/GeoModelIOConfig.cmake.in
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   @ONLY )
install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Set up the packaging of the project using CPack.
include( GeoModelIOPackaging )
