# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1Gui/*.h )
file( GLOB UIS src/*.ui VP1Gui/*.ui )
file( GLOB RCS src/*.qrc )

# Add the library.
add_library( GXGui SHARED ${SOURCES} ${HEADERS} ${UIS} ${RCS} )
target_link_libraries( GXGui
   PUBLIC Coin::Coin Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets SoQt::SoQt GXBase
   PRIVATE Qt${QT_VERSION}::PrintSupport Qt${QT_VERSION}::OpenGL )
target_include_directories( GXGui PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "VP1Gui" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} ${UIS} ${RCS} )
set_target_properties( GXGui PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( GEOMODEL_USE_BUILTIN_COIN3D )
   add_dependencies( GXGui Coin3DBuiltIn SoQtBuiltIn )
endif()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelVisualization::GXGui ALIAS GXGui )

# Install the library.
install( TARGETS GXGui
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_COMPONENT Development )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1Gui
   COMPONENT Development )
