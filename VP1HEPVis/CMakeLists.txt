
# External dependencies:
find_package( Coin  REQUIRED )
find_package( OpenGL REQUIRED )


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1HEPVis/*.h )


# Configure package build.
add_library ( VP1HEPVis SHARED ${SOURCES} ${HEADERS}  )
target_link_libraries( VP1HEPVis PUBLIC Qt5::Core
GeoModelCore::GeoModelKernel Coin::Coin )
if( APPLE )
  target_link_libraries (VP1HEPVis PUBLIC ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} )
else()
  target_link_libraries (VP1HEPVis PUBLIC )
endif()


# Configure debug output.
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
   target_compile_definitions( VP1HEPVis PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()


# Configure package installation.
target_include_directories( VP1HEPVis PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "VP1HEPVis" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1HEPVis PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


# Install the library.
install( TARGETS VP1HEPVis
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS VP1HEPVis
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1HEPVis
   COMPONENT Development )
