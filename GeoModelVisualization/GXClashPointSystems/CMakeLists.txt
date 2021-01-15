# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GXClashPointSystems/*.h )
file( GLOB UIS src/*.ui )

# Add the library.
add_library( GXClashPointSystems SHARED ${SOURCES} ${HEADERS} ${UIS} )
target_link_libraries( GXClashPointSystems
   PUBLIC Coin::Coin GXBase
   PRIVATE Qt5::Core Qt5::Widgets nlohmann_json::nlohmann_json )
target_include_directories( GXClashPointSystems PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GXClashPointSystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} ${RCS} )
set_target_properties( GXClashPointSystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXClashPointSystems Coin3DBuiltIn )
endif()
if( GEOMODEL_USE_BUILTIN_JSON )
   add_dependencies( GXClashPointSystems JSONExt )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXClashPointSystems ALIAS
   GXClashPointSystems )

# Install the library.
install( TARGETS GXClashPointSystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GXClashPointSystems
   COMPONENT Development )
