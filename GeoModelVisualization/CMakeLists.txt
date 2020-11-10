# Set up the project.
cmake_minimum_required( VERSION 3.10 )

project( "GeoModelVisualization" VERSION 3.2.0 LANGUAGES CXX )
# set( CMAKE_BUILD_TYPE RELEASE )

# Set the OpenGL flag requested by CMake
set( OpenGL_GL_PREFERENCE GLVND )

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Set up how the project handle some of its dependenices. Either by picking them
    # up from the environment, or building them itself.
    include( SetupJSON )
    # Find the base GeoModel packages, which must be installed on the target system already
    find_package( GeoModelCore REQUIRED 4.0.0 ) #TODO: the version should be se in the root folder
    find_package( GeoModelIO REQUIRED 4.0.0 ) #TODO: the version should be se in the root folder
    # Set a flag to steer the  of the subpackages
    set( VISUALIZATION_INDIVIDUAL_BUILD ON )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModel project.")
endif()

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
