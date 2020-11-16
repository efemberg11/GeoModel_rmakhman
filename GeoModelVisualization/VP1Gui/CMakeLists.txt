# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1Gui/*.h )
file( GLOB UIS src/*.ui VP1Gui/*.ui )
file( GLOB RCS src/*.qrc )

# Add the library.
add_library( GXGui SHARED ${SOURCES} ${HEADERS} ${UIS} ${RCS} )
target_link_libraries( GXGui
   PUBLIC Coin::Coin Qt5::Core Qt5::Gui Qt5::Widgets SoQt::SoQt GXBase
   PRIVATE Qt5::PrintSupport Qt5::OpenGL )
target_include_directories( GXGui PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
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
   NAMELINK_SKIP )
install( TARGETS GXGui
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1Gui
   COMPONENT Development )
