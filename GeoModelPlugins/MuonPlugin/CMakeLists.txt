# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# External dependencies
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( XercesC REQUIRED )
find_package( ZLIB REQUIRED )


# Create the library.
add_library( MuonPlugin SHARED ${SOURCES} )
target_link_libraries( MuonPlugin PUBLIC GeoModelCore::GeoModelKernel XercesC::XercesC GeoModelXml )
target_include_directories( MuonPlugin PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "MuonPlugin" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( MuonPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelPlugins::MuonPlugin ALIAS MuonPlugin )

# Install the library.
install(TARGETS MuonPlugin
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonPlugin
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MuonPlugin
   COMPONENT Development )

# set ( PLUGIN_NAME  "MuonPlugin.so" )
   
# ADD_CUSTOM_TARGET( Muon_plugin ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${MuonPlugin} ${PLUGIN_NAME} )


