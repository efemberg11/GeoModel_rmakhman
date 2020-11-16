# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS TFPersistification/*.h )

# Set up the library.
add_library( TFPersistification SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( TFPersistification PUBLIC
GeoGenericFunctions GeoModelKernel )
target_include_directories( TFPersistification PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "TFPersistification" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( TFPersistification PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelIO::TFPersistification ALIAS TFPersistification )

# Install the library.
install( TARGETS TFPersistification
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS TFPersistification
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TFPersistification
   COMPONENT Development )
