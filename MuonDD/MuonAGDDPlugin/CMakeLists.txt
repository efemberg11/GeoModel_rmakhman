# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS MuonAGDDPlugin/*.h )

# Create the library.
add_library( MuonAGDDPlugin SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( MuonAGDDPlugin PUBLIC GeoModelCore::GeoModelKernel AGDDHandlers AGDDControl GeoModelTools::GeoModelXMLParser MuonAGDDBase GeoXmlMatManager )
target_include_directories( MuonAGDDPlugin PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "MuonAGDDPlugin" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( MuonAGDDPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelATLAS::MuonAGDDPlugin ALIAS MuonAGDDPlugin )

# Install the library.
install(TARGETS MuonAGDDPlugin
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonAGDDPlugin
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonAGDDPlugin
   COMPONENT Development )


