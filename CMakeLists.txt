# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelIO" VERSION 3.2.0 LANGUAGES CXX )

# ---------------------------

# Make the module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Set build and C++ default options
include ( configure_cpp_options )

# Set optional definitions to steer the source code
include( configure_definitions)

# Use the GNU install directory names.
include( GNUInstallDirs )

# Print Build Info on screen
include( PrintBuildInfo )

# ---------------------------

# Project's dependencies.
find_package( GeoModelCore 3.2.0 REQUIRED )
find_package( Qt5 COMPONENTS Core Sql REQUIRED )

# Set sub-packages to build.
add_subdirectory( GeoModelErrorHandler )
add_subdirectory( GeoModelDBManager )
add_subdirectory( TFPersistification )
add_subdirectory( GeoModelRead )
add_subdirectory( GeoModelWrite )
#

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
