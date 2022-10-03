# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GXTrackDisplaySystems/*.h )
file( GLOB UIS src/*.ui )

# Add the library.
add_library( GXTrackDisplaySystems SHARED ${SOURCES} ${HEADERS} ${UIS} )
target_link_libraries( GXTrackDisplaySystems
   PUBLIC Coin::Coin GXBase 
   PRIVATE Qt5::Core Qt5::Widgets ${HDF5_LIBRARIES} )
target_include_directories( GXTrackDisplaySystems PUBLIC
   ${HDF5_CXX_INCLUDE_DIRS} 
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GXTrackDisplaySystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} ${RCS} )
set_target_properties( GXTrackDisplaySystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXTrackDisplaySystems Coin3DBuiltIn )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXTrackDisplaySystems ALIAS
   GXTrackDisplaySystems )

# Install the library.
install( TARGETS GXTrackDisplaySystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GXClashPointSystems
   COMPONENT Development )
