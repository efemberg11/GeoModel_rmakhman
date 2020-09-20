# Set up the project.
cmake_minimum_required( VERSION 3.10 )

project( "GeoModelVisualization" VERSION 3.2.0 LANGUAGES CXX )
# set( CMAKE_BUILD_TYPE RELEASE )

# Set the OpenGL flag requested by CMake
set( OpenGL_GL_PREFERENCE GLVND )


# Make the module directory visible to CMake.
# list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# Use the GNU install directory names.
include( GNUInstallDirs )

# External dependencies.
# find_package( nlohmann_json REQUIRED )

# Set sub-packages to build.
add_subdirectory(VP1GeometryPlugin)
add_subdirectory(VP1GeometrySystems)
add_subdirectory(GXClashPointSystems)
add_subdirectory(GeoModelExplorer)
add_subdirectory(VP1Gui)
add_subdirectory(VP1Base)
add_subdirectory(VP1HEPVis)
add_subdirectory(VP1GuideLineSystems)
