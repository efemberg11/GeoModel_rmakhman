# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS ExpressionEvaluator/*.h )

# Create the library.
add_library( ExpressionEvaluator SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( ExpressionEvaluator PRIVATE
   GeoModelCore::GeoModelKernel )
target_include_directories( ExpressionEvaluator PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "ExpressionEvaluator" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( ExpressionEvaluator PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS ExpressionEvaluator
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS ExpressionEvaluator
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ExpressionEvaluator
   COMPONENT Development )
