# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometrySystems/*.h )
file( GLOB UIS src/*.ui )
file( GLOB DATA share/*.jpg share/*.json )

# Add the library.
add_library( GXGeometrySystems SHARED ${SOURCES} ${HEADERS} ${UIS} )
target_link_libraries( GXGeometrySystems
   PUBLIC ${COIN3D_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets
          GeoModelCore::GeoModelKernel GXBase
   PRIVATE nlohmann_json::nlohmann_json GeoModelIO::GeoModelRead
           GeoModelIO::GeoModelWrite GeoModelIO::GeoModelDBManager GXHEPVis )
target_include_directories( GXGeometrySystems PUBLIC ${COIN3D_INCLUDE_DIRS}
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
target_compile_definitions( GXGeometrySystems PRIVATE
   GEOMODEL_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}" )
source_group( "VP1GeometrySystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} ${RCS} )
set_target_properties( GXGeometrySystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXGeometrySystems ALIAS GXGeometrySystems )

# Install the library.
install( TARGETS GXGeometrySystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GXGeometrySystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1GeometrySystems
   COMPONENT Development )

# Install additional files from the package.
install( FILES ${DATA}
   DESTINATION ${CMAKE_INSTALL_DATADIR}/gmex )
