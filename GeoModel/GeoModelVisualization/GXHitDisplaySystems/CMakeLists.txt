# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GXHitDisplaySystems/*.h )
file( GLOB UIS src/*.ui )
find_package (HDF5 REQUIRED)
# Add the library.
add_library( GXHitDisplaySystems SHARED ${SOURCES} ${HEADERS} ${UIS} )
target_link_libraries( GXHitDisplaySystems
   PUBLIC Coin::Coin GXBase
   PRIVATE Qt${QT_VERSION}::Core Qt${QT_VERSION}::Widgets ${HDF5_CXX_LIBRARIES} ${HDF5_LIBRARIES} )
target_include_directories( GXHitDisplaySystems PUBLIC
   ${HDF5_CXX_INCLUDE_DIRS}
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GXHitDisplaySystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} ${RCS} )
set_target_properties( GXHitDisplaySystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXHitDisplaySystems Coin3DBuiltIn )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXHitDisplaySystems ALIAS
   GXHitDisplaySystems )

# Install the library.
install( TARGETS GXHitDisplaySystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GXClashPointSystems
   COMPONENT Development )
