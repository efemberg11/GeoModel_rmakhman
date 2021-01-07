# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS MuonAGDDBase/*.h )

# Create the library.
add_library( MuonAGDDBase SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( MuonAGDDBase PUBLIC GeoModelCore::GeoModelKernel AGDDKernel XercesC::XercesC GeoModelTools::ExpressionEvaluator MuonAGDDDescription MuonGeoModel GeoModelTools::GeoModelXMLParser )
target_include_directories( MuonAGDDBase PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "MuonAGDDBase" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( MuonAGDDBase PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelATLAS::MuonAGDDBase ALIAS MuonAGDDBase )

# Install the library.
install(TARGETS MuonAGDDBase
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonAGDDBase
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonAGDDBase
   COMPONENT Development )

