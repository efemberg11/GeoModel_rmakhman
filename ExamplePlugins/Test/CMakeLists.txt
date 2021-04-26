cmake_minimum_required(VERSION 3.12)
set( CMAKE_CXX_FLAGS "-O0" )
project( "GeoModelPlugins-Test" )


#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )
    # get global GeoModel version
    include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "GeoModelPlugins-Test" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Find the base GeoModelATLAS packages, which must be installed on the target system already
    find_package( GeoModelDataManagers REQUIRED ${GeoModelATLAS_VERSION} )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModelATLAS project.")
    # Set the project
    project( "GeoModelPlugins-Test" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
endif()

# Other project's dependencies.
# GeoModel dependencies
find_package( GeoModelCore REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )


add_executable ( toytest src/toytest.cxx )
target_link_libraries ( toytest GeoModelCore::GeoModelKernel GeoModelDataManagers::GeoXmlMatManager )

install( TARGETS toytest DESTINATION bin )
