# Set up the project.
cmake_minimum_required( VERSION 3.1...3.19 )

project( "MagneticField" VERSION ${GeoModel_VERSION} LANGUAGES CXX )

# Set up the build of the libraries of the project's subpackages
add_subdirectory(MagFieldInterfaces)


# Install headers 
file( GLOB HEADERS_Int MagFieldInterfaces/MagFieldInterfaces/*.h )
install( FILES ${HEADERS_Int}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MagFieldInterfaces
   COMPONENT Development )

# install the CMake configuration files of the subpackages
install(EXPORT MagFieldInterfaces-export FILE MagneticField-MagFieldInterfaces.cmake DESTINATION lib/cmake/MagneticField)

# Write and install the version file
write_basic_config_version_file(
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion )
install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Install the hand-written project configuration.
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MagneticFieldConfig.cmake.in
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   @ONLY )
install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

