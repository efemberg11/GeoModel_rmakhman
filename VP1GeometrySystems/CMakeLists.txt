################################################################################
# Package: GeoModelGeometrySystems
################################################################################

# # External dependencies:
# find_package( Coin  REQUIRED )
# find_package( OpenGL REQUIRED )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometrySystems/*.h )
file( GLOB UIS src/*.ui VP1GeometrySystems/*.ui )


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/VP1GeometrySystems/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


# Configure package build.
add_library ( VP1GeometrySystems SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )
target_link_libraries (VP1GeometrySystems
  VP1GUI VP1Base VP1HEPVis
  GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite
  Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport Qt5::Sql)

# Configure debug output
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
      "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
    target_compile_definitions( VP1GeometrySystems PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()

# Configure package installation.
target_include_directories( VP1GeometrySystems PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "VP1GeometrySystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1GeometrySystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )



# Install the library.
install( TARGETS VP1GeometrySystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS VP1GeometrySystems
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1GeometrySystems
   COMPONENT Development )
