# $Id: CMakeLists.txt 732131 2016-03-24 11:03:29Z krasznaa $
################################################################################
# Package: GeoModelGeometryPlugin
################################################################################

# cmake_minimum_required(VERSION 3.12)

# set(CMAKE_BUILD_TYPE RELEASE)
# #set(CMAKE_CXX_FLAGS "-g -O0 -gdwarf-2")
#
# set(MYLIB_VERSION_MAJOR 1)
# set(MYLIB_VERSION_MINOR 0)
# set(MYLIB_VERSION_PATCH 0)
# project ( "VP1GeometrySystems" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometrySystems/*.h )
file( GLOB UIS src/*.ui VP1GeometrySystems/*.ui )


# if ( APPLE )
# 	find_package(OpenGL REQUIRED)
# endif()
#find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )

# find_package( Coin3D )
# find_package( Qt5 COMPONENTS Core Gui Widgets Sql )
# find_package( GeoModelCore REQUIRED )
# find_package( GeoModelIO REQUIRED )
# find_package( Eigen3 REQUIRED )
# if( APPLE )
#   find_package(OpenGL REQUIRED)
# endif()



set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/VP1GeometrySystems/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


# include_directories ("${EIGEN3_INCLUDE_DIR}")
# include_directories ("${PROJECT_SOURCE_DIR}")
# include_directories ("${PROJECT_SOURCE_DIR}/src")
# include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
# include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")

# External dependencies:
# include_directories(${Qt5Core_INCLUDE_DIRS} )
# include_directories(${Qt5Gui_INCLUDE_DIRS} )
# include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
# include_directories(${Qt5Widgets_INCLUDE_DIRS} )
# include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
# include_directories(${Qt5Network_INCLUDE_DIRS} )
# include_directories(${Qt5Sql_INCLUDE_DIRS} )
# include_directories(${Coin3D_INCLUDE_DIRS} )
# include_directories(${SoQt_INCLUDE_DIRS} )
# include_directories(${VP1HEPVis_INCLUDE_DIRS} )
# add_definitions (${Qt5Core_DEFINITIONS})


# Configure package build.
add_library ( VP1GeometrySystems SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )
target_link_libraries (VP1GeometrySystems
  VP1GUI VP1Base VP1HEPVis
  GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite
  SoQt Coin
  Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport Qt5::Sql)


# Configure package installation.
target_include_directories( VP1GeometrySystems PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
#TODO: unify the Coin/Coin3D name for our Apple/Ubuntu packages!!
source_group( "VP1GeometrySystems" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1GeometrySystems PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
   target_compile_definitions( VP1GeometrySystems PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()



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

# install(TARGETS VP1GeometrySystems
#   LIBRARY
#   DESTINATION lib
#   COMPONENT Libraries
#   )
#
# set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})
#
# set_target_properties(VP1GeometrySystems PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
