# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometryPlugin/*.h )

# Add the library.
add_library( GXGeometryPlugin MODULE ${SOURCES} ${HEADERS} )
target_link_libraries( GXGeometryPlugin
   PUBLIC Qt${QT_VERSION}::Core GXBase
   PRIVATE GXGeometrySystems GXGuideLineSystems GXClashPointSystems GXHitDisplaySystems GXTrackDisplaySystems)
target_include_directories( GXGeometryPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "VP1GeometryPlugin" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXGeometryPlugin ALIAS GXGeometryPlugin )

# Install the library.
install( TARGETS GXGeometryPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gxplugins
   COMPONENT Runtime 
   NAMELINK_COMPONENT Development )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1GeometryPlugin
   COMPONENT Development )
