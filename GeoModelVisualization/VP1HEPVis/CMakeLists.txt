# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1HEPVis/*.h )

# Add the library.
add_library( GXHEPVis SHARED ${SOURCES} ${HEADERS} )
target_include_directories( GXHEPVis PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
target_link_libraries( GXHEPVis
   PUBLIC Coin::Coin OpenGL::GL GeoModelCore::GeoModelKernel)
source_group( "VP1HEPVis" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GXHEPVis PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXHEPVis Coin3DBuiltIn )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXHEPVis ALIAS GXHEPVis )

# Install the library.
install( TARGETS GXHEPVis
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( DIRECTORY VP1HEPVis
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   COMPONENT Development )
