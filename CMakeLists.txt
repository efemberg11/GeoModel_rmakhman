# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelIO" VERSION 1.3.0 LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "Require the specified C++ standard for the build" )

# Project's dependencies.
find_package( GeoModelCore REQUIRED )
find_package( Qt5 COMPONENTS Core Sql REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Set sub-packages to build.
add_subdirectory( GeoModelErrorHandler )
add_subdirectory( GeoModelDBManager )
add_subdirectory( TFPersistification )
add_subdirectory( GeoModelRead )
add_subdirectory( GeoModelWrite )

# Create and install the version description of the project.
include( WriteBasicConfigVersionFile )
write_basic_config_version_file(
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Version.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion )
install(
   FILES ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Version.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Create and install the description of the libraries.
install( EXPORT ${PROJECT_NAME}-export
   FILE ${PROJECT_NAME}Targets.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Install the hand-written project configuration.
configure_file( ${CMAKE_SOURCE_DIR}/cmake/GeoModelIOConfig.cmake.in
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/GeoModelIOConfig.cmake @ONLY )
install(
   FILES ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/GeoModelIOConfig.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )
