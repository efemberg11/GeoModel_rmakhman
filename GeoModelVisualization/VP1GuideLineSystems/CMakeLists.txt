# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GuideLineSystems/*.h )
file( GLOB UIS src/*.ui )

# Add the library.
add_library( GXGuideLineSystems SHARED ${SOURCES} ${HEADERS} ${UIS} )
target_link_libraries( GXGuideLineSystems
   PUBLIC Coin::Coin Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets GXBase
   PRIVATE GeoModelCore::GeoModelKernel GXHEPVis )
target_include_directories( GXGuideLineSystems PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "VP1GuideLineSystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} )
set_target_properties( GXGuideLineSystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXGuideLineSystems Coin3DBuiltIn )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXGuideLineSystems ALIAS
  GXGuideLineSystems )

# Install the library.
install( TARGETS GXGuideLineSystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1GuideLineSystems
   COMPONENT Development )
