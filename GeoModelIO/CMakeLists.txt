# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Set up the (sub-)project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelIO" VERSION ${GeoModel_VERSION} LANGUAGES CXX )

# Set up the version of GeoModelIO as a cache variable, so that other
# sub-projects could use this value.
#set( GeoModelIO_VERSION "${GeoModelIO_VERSION}" CACHE STRING
#   "Version of the GeoModelIO (sub-)project" )

# Set sub-packages to build.
# add_subdirectory( GeoModelErrorHandler ) // unused at the moment.
add_subdirectory( GeoModelDBManager )
add_subdirectory( TFPersistification )
add_subdirectory( GeoModelRead )
add_subdirectory( GeoModelWrite )

# Create and install the version description of the project.
include( CMakePackageConfigHelpers )
write_basic_config_version_file(
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion )
install(
   FILES
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Create and install the description of the libraries.
export( EXPORT ${PROJECT_NAME}-export
   NAMESPACE "${PROJECT_NAME}::"
   FILE ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Targets.cmake )
install( EXPORT ${PROJECT_NAME}-export
   FILE ${PROJECT_NAME}Targets.cmake
   COMPONENT Development
   NAMESPACE "${PROJECT_NAME}::"
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Install the hand-written project configuration.
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GeoModelIOConfig.cmake.in
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   @ONLY )
install(
   FILES
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Set up the packaging of the project using CPack.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
include( GeoModelIOPackaging )
