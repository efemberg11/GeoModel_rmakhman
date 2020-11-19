# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelXMLParser/*.h )

# Create the library.
add_library( GeoModelXMLParser SHARED ${HEADERS} ${SOURCES} )
# If the in-house build of the Xerces-C library is used, add explicit dependency
if( GEOMODEL_USE_BUILTIN_XERCESC )
    add_dependencies( GeoModelXMLParser XercesCBuiltIn )
endif()
target_link_libraries( GeoModelXMLParser PUBLIC XercesC::XercesC
   ExpressionEvaluator )
target_include_directories( GeoModelXMLParser PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GeoModelXMLParser" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelXMLParser PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS GeoModelXMLParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     NAMELINK_COMPONENT Development
   NAMELINK_ONLY
     COMPONENT Development )
#install( TARGETS GeoModelXMLParser
#   EXPORT ${PROJECT_NAME}-export
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   COMPONENT Development
#   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelXMLParser
   COMPONENT Development )
