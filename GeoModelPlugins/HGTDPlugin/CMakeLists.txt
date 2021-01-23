# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "HGTDPlugin" )


#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )
    # get global GeoModel version
    include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "HGTDPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
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
    if( NOT TARGET XercesCBuiltIn )
        include( SetupXercesC )
    endif()
    # Find the base GeoModelATLAS packages, which must be installed on the target system already
    find_package( GeoModelDataManagers REQUIRED ${GeoModelATLAS_VERSION} )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModelATLAS project.")
    # Set the project
    project( "HGTDPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
endif()

# Project's dependencies.
# GeoModel dependencies
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
# Third-party dependencies
find_package( XercesC REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set up the library.
add_library( HGTDPlugin SHARED ${SOURCES} )
target_link_libraries( HGTDPlugin PUBLIC GeoModelCore::GeoModelKernel GeoModelDataManagers::GeoXmlMatManager GeoModelTools::GeoModelXMLParser )
target_include_directories( HGTDPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "src" FILES ${SOURCES} )
set_target_properties( HGTDPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS HGTDPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
