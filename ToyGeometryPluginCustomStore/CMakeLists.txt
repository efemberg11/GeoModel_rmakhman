# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "ToyGeometryPluginCustomStore" VERSION 1.0.0 LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

find_package( Eigen3 REQUIRED )
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( GeoModelDataManagers REQUIRED )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set up the library.
add_library( ToyGeometryPluginCustomStore SHARED ${SOURCES} )
target_link_libraries( ToyGeometryPluginCustomStore PUBLIC GeoModelCore::GeoModelKernel GeoXmlMatManager GeoModelTools::GeoModelXMLParser )
target_include_directories( ToyGeometryPluginCustomStore PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "src" FILES ${SOURCES} )
set_target_properties( ToyGeometryPluginCustomStore PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS ToyGeometryPluginCustomStore
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
