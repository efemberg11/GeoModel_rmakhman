# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS MuonAGDDDescription/*.h )

# Create the library.
add_library( MuonAGDDDescription SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( MuonAGDDDescription PUBLIC GeoModelCore::GeoModelKernel AGDDKernel AGDDModel )
target_include_directories( MuonAGDDDescription PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "MuonAGDDDescription" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( MuonAGDDDescription PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelATLAS::MuonAGDDDescription ALIAS MuonAGDDDescription )

# Install the library.
install(TARGETS MuonAGDDDescription
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonAGDDDescription
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonAGDDDescription
   COMPONENT Development )

