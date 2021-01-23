# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "ToyXMLAuxDataPlugin" )

#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )
    # get global GeoModel version
    include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "ToyXMLAuxDataPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
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
    project( "ToyXMLAuxDataPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
endif()

# Project's dependencies.
# GeoModel dependencies
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set up the library.
add_library( ToyXMLAuxDataPlugin SHARED ${SOURCES} )
target_link_libraries( ToyXMLAuxDataPlugin PUBLIC GeoModelCore::GeoModelKernel GeoModelDataManagers::GeoXmlMatManager GeoModelDataManagers::GeoXmlInpManager GeoModelTools::GeoModelXMLParser )
target_include_directories( ToyXMLAuxDataPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "src" FILES ${SOURCES} )
set_target_properties( ToyXMLAuxDataPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS ToyXMLAuxDataPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
