# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

find_package( AGDD REQUIRED )
find_package( CLHEP REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( XercesC REQUIRED )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set up the library.
add_library( AGDDPlugin SHARED ${SOURCES} )
target_link_libraries( AGDDPlugin PUBLIC GeoModelCore::GeoModelKernel AGDDHandlers AGDDControl XMLParser )
target_include_directories( AGDDPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "src" FILES ${SOURCES} )
set_target_properties( AGDDPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS AGDDPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
