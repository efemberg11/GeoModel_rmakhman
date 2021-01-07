# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

cmake_minimum_required( VERSION 3.12 )

# Set up the project.
project( "MuonDD" VERSION 1.0.0 LANGUAGES CXX )

# --- External dependencies ---
# GeoModel dependencies
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
# Third-party dependencies
find_package( XercesC REQUIRED )


# --- Add sub-projects ---
add_subdirectory(MuonAGDDDescription)
add_subdirectory(MuonGeoModel)
add_subdirectory(MuonAGDDBase)
add_subdirectory(MuonAGDDPlugin)


# -- Export & Install the project --
# Create and install the version description of the project.
include( WriteBasicConfigVersionFile )
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
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MuonDDConfig.cmake.in
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   @ONLY )
install(
   FILES
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Set up the packaging of the project using CPack.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
include( MuonDDPackaging )



#install(EXPORT MuonAGDDBase-export FILE MuonDD-MuonAGDDBase.cmake DESTINATION lib/cmake/MuonDD)
#install(EXPORT MuonGeoModel-export FILE MuonDD-MuonGeoModel.cmake DESTINATION lib/cmake/MuonDD)
#install(EXPORT MuonAGDDDescription-export FILE MuonDD-MuonAGDDDescription.cmake DESTINATION lib/cmake/MuonDD)

#install(FILES cmake/MuonDDConfig.cmake DESTINATION lib/cmake/MuonDD)

