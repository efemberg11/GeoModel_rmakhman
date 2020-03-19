# Set up the project.
cmake_minimum_required( VERSION 3.10 )
set  (CMAKE_CXX_STANDARD 14)
project( "GeoModelVisualization" VERSION 3.1.2 LANGUAGES CXX )
set    ( CMAKE_BUILD_TYPE RELEASE )

set    (OpenGL_GL_PREFERENCE GLVND)


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

add_subdirectory(VP1GuideLineSystems)

