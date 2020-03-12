# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS XMLParser/*.h )

# Create the library.
add_library( XMLParser SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( XMLParser PUBLIC XercesC::XercesC ExpressionEvaluator )
target_include_directories( XMLParser SYSTEM PUBLIC )
target_include_directories( XMLParser PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "XMLParser" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( XMLParser PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_XERCESC )
   add_dependencies( XMLParser XercesC )
endif()

# Install the library.
install( TARGETS XMLParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS XMLParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/XMLParser
   COMPONENT Development )
