# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1HEPVis/*.h )

# Add the library.
add_library( GXHEPVis SHARED ${SOURCES} ${HEADERS} )
target_include_directories( GXHEPVis PUBLIC ${COIN3D_INCLUDE_DIRS}
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
target_link_libraries( GXHEPVis
   PUBLIC ${COIN3D_LIBRARIES} OpenGL::GL )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXHEPVis ALIAS GXHEPVis )

# Install the library.
install( TARGETS GXHEPVis
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GXHEPVis
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( DIRECTORY VP1HEPVis
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   COMPONENT Development )
