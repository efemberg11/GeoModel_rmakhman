# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelXml/*.h )
file( GLOB SHAPEHEADERS GeoModelXml/shape/*.h )
file( GLOB DATA data/*.dtd )

# External dependencies
find_package( ZLIB REQUIRED )


# Create the library.
add_library( GeoModelXml SHARED ${HEADERS} ${SOURCES} )
# link libraries
target_link_libraries( GeoModelXml PUBLIC GeoModelCore::GeoModelKernel GeoModelCore::GeoModelHelpers GeoModelTools::ExpressionEvaluator XercesC::XercesC PRIVATE ZLIB::ZLIB  )
target_include_directories( GeoModelXml PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GeoModelXml" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelXml PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelTools::GeoModelXml ALIAS GeoModelXml )

# Install the library.
install(TARGETS GeoModelXml
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelXml
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelXml
    COMPONENT Development )
install( FILES ${SHAPEHEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelXml/shape)
install( FILES ${DATA} DESTINATION ${CMAKE_INSTALL_DATADIR}/GeoModelXml)
