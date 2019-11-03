# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelVisualization" VERSION 1.0.0 LANGUAGES CXX )
set    ( CMAKE_BUILD_TYPE RELEASE )
# Project's dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO   REQUIRED )
find_package( Qt5 COMPONENTS Core Gui Widgets Sql )
# Set sub-packages to build.
add_subdirectory(VP1GeometryPlugin)
add_subdirectory(VP1GeometrySystems)
add_subdirectory(GeoModelExplorer)
add_subdirectory(VP1Gui)
add_subdirectory(VP1Base)
add_subdirectory(VP1HEPVis)
add_subdirectory(VP1UtilsBase)

