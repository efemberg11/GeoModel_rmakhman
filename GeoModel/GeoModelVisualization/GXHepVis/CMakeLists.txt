# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GXHepVis/*.h )

# Add the library.
add_library( GXHepVis SHARED ${SOURCES} ${HEADERS} )
target_include_directories( GXHepVis PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
target_link_libraries( GXHepVis
    PUBLIC Coin::Coin OpenGL::GL GeoModelCore::GeoModelKernel)
source_group( "GXHepVis" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GXHepVis PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXHepVis Coin3DBuiltIn )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXHepVis ALIAS GXHepVis )

# Install the library.
install( TARGETS GXHepVis
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( DIRECTORY GXHepVis
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   COMPONENT Development )
