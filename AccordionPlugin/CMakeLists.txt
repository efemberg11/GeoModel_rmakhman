# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set up the library.
add_library( AccordionPlugin SHARED ${SOURCES} )
target_link_libraries( AccordionPlugin PUBLIC GeoModelCore::GeoModelKernel )
target_include_directories( AccordionPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "src" FILES ${SOURCES} )
set_target_properties( AccordionPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS AccordionPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
