# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

find_package( GMX REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( XercesC REQUIRED )
find_package( ZLIB REQUIRED )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set up the library.
add_library( GMXPlugin SHARED ${SOURCES} )
target_link_libraries( GMXPlugin PUBLIC GeoModelCore::GeoModelKernel GeoModelXml )
target_include_directories( GMXPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GMXPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS GMXPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
