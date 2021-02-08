# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration

cmake_minimum_required( VERSION 3.12 )
project( "ITKGeoPlugin" )


#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )
    # get global GeoModel version
    include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "ITKGeoPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
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
    find_package( GeoModelXML REQUIRED )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModelATLAS project.")
    # Set the project
    project( "ITKPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
endif()

# GeoModel dependencies
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
# External dependencies
find_package( XercesC REQUIRED )
find_package( ZLIB REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )


# Find the header and source files.
file( GLOB SOURCES src/*.cxx)
include_directories( ../GeoModelPlugins )
# Create the library.
add_library( ITKPlugin SHARED ${SOURCES} )
target_link_libraries( ITKPlugin PUBLIC GeoModelCore::GeoModelKernel XercesC::XercesC GeoModelXML::GeoModelXml )
set_target_properties( ITKPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

install( TARGETS ITKPlugin
	 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	 COMPONENT Runtime
	 NAMELINK_SKIP )


