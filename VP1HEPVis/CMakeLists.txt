# cmake_minimum_required(VERSION 3.12)

# set(MYLIB_VERSION_MAJOR 1)
# set(MYLIB_VERSION_MINOR 0)
# set(MYLIB_VERSION_PATCH 0)

# project ( "GXHEPVis" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

# External dependencies:
#TODO: unify the Coin/Coin3D name for our Apple/Ubuntu packages!!
if ( APPLE )
  find_package( Coin  REQUIRED )
	find_package( OpenGL REQUIRED )
else()
  find_package( Coin3D  REQUIRED )
endif()




# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1HEPVis/*.h )

# TODO: to be cleaned up...
# include_directories("${PROJECT_SOURCE_DIR}")
# include_directories(${Qt5Core_INCLUDE_DIRS} )
# add_definitions (${Qt5Core_DEFINITIONS})
# if ( APPLE )
#   include_directories(${Coin_INCLUDE_DIR} )
# else()
#   include_directories(${Coin3D_INCLUDE_DIRS} )
# endif()

# message("Coin_INCLUDE_DIR: ${Coin_INCLUDE_DIR}")

# Configure package build.
add_library ( VP1HEPVis SHARED ${SOURCES} ${HEADERS}  )
target_link_libraries( VP1HEPVis PUBLIC Qt5::Core
GeoModelCore::GeoModelKernel )


# Configure package installation.
target_include_directories( VP1HEPVis PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
#TODO: unify the Coin/Coin3D name for our Apple/Ubuntu packages!!
if( APPLE )
  target_link_libraries (VP1HEPVis PUBLIC ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} Coin)
else()
  target_link_libraries (VP1HEPVis PUBLIC Coin3D)
endif()
source_group( "VP1HEPVis" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1HEPVis PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
   target_compile_definitions( VP1HEPVis PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()

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



# # Install
# install(TARGETS VP1HEPVis
#   LIBRARY
#   DESTINATION lib
#   COMPONENT Libraries
#   )
#
# # Set version number for installed library
# set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})
# set_target_properties(VP1HEPVis PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
