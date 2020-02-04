cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "GeoModelVisualization" VERSION 1.0.0 LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "Require the specified C++ standard for the build" )

# # Set a subfolder where to install the visualization plugins.
# set( ENV{GEOMODEL_VIS_PLUGINFOLDER} GeoModelPluginsViz)
# set( GEOMODEL_DIR_PLUGINS ${CMAKE_INSTALL_LIBDIR}/$ENV{GEOMODEL_VIS_PLUGINFOLDER}
#   CACHE PATH "The folder used to install the GeoModel visualization plugins" )
# # message("GEOMODEL_EXPLORER_PLUGINPATH: $ENV{GEOMODEL_EXPLORER_PLUGINPATH}")

# Make the module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Add definition to build/exclude specific parts of the code
# ADD_DEFINITIONS(-DBUILDGEOMODELEXPLORER=1)

# Project's dependencies.
find_package( GeoModelCore 2.0.0 REQUIRED )
find_package( GeoModelIO   2.0.0 REQUIRED )
find_package( Qt5 COMPONENTS Core Gui Widgets PrintSupport Network )
if( APPLE )
  find_package(OpenGL REQUIRED)
endif()


# Use the GNU install directory names.
include( GNUInstallDirs )

# Set sub-packages to build.
add_subdirectory(VP1GeometryPlugin) # FIXME: to be renamed, e.g. 'GeoModelGeometryPlugin'
add_subdirectory(VP1GeometrySystems) # FIXME: to be renamed, e.g. 'GeoModelGeometrySystem'
add_subdirectory(GeoModelExplorer)
add_subdirectory(VP1Gui) # FIXME: to be renamed, e.g. 'GeoModelGUI'
add_subdirectory(VP1Base)
add_subdirectory(VP1HEPVis)

# Create and install the version description of the whole project.
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
configure_file( ${CMAKE_SOURCE_DIR}/cmake/GeoModelVisualizationConfig.cmake.in
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   @ONLY )
install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Set up the packaging of the project using CPack.
include( GeoModelVisualizationPackaging )
