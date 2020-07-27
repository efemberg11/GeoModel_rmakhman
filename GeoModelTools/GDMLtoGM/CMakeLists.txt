# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GDMLInterface/*.h )

set(OUTPUT bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT})

# Create the library.
add_library( GDMLtoGM SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GDMLtoGM PUBLIC XercesC::XercesC
   ExpressionEvaluator GeoModelXMLParser GeoModelKernel )
target_include_directories( GDMLtoGM PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GDMLtoGM" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GDMLtoGM PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_XERCESC )
   add_dependencies( GDMLtoGM XercesC )
endif()

# Install the library.
install( TARGETS GDMLtoGM
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GDMLtoGM
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GDMLInterface
   COMPONENT Development )
   
add_executable(gdml2gm gdml2gm/gdml2gm.cxx ${sources} ${headers})
if( GEOMODEL_USE_BUILTIN_XERCESC )
   add_dependencies( gdml2gm XercesC )
endif()
target_link_libraries(gdml2gm XercesC::XercesC
   ExpressionEvaluator GeoModelXMLParser GeoModelKernel GeoModelWrite GeoModelDBManager GDMLtoGM)
install(TARGETS gdml2gm DESTINATION ${OUTPUT})
