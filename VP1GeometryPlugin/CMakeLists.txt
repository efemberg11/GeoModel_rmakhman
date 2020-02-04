################################################################################
# Package: VP1GeometryPlugin
################################################################################
# cmake_minimum_required(VERSION 3.12)

# set(MYLIB_VERSION_MAJOR 1)
# set(MYLIB_VERSION_MINOR 0)
# set(MYLIB_VERSION_PATCH 0)
#
# project ( "GXGeometryPlugin" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

# if ( APPLE )
# 	find_package(OpenGL REQUIRED)
# endif()
#find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )

# FIXME: the double-naming of Coin packages
# External dependencies:
if( APPLE )
  find_package( Coin )
else()
  find_package( Coin3D )
endif()

# find_package( Qt5 COMPONENTS Core Gui Widgets Sql )
# find_package( GeoModelCore REQUIRED )
# find_package( GeoModelIO REQUIRED )
# find_package( Eigen3 REQUIRED )
# if( APPLE )
#   find_package(OpenGL REQUIRED)
# endif()

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometryPlugin/*.h )
file( GLOB UIS src/*.ui VP1GeometryPlugin/*.ui )


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


# include_directories ("${EIGEN3_INCLUDE_DIR}")
# include_directories ("${PROJECT_SOURCE_DIR}")
# include_directories ("${PROJECT_SOURCE_DIR}/src")
# include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
# include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")
# include_directories ("${PROJECT_SOURCE_DIR}/../GeoModelGeometrySystem")

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
add_library ( GXGeometryPlugin SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )
target_link_libraries (GXGeometryPlugin VP1GeometrySystems VP1GUI VP1Base VP1HEPVis SoQt Coin Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport )

# Configure debug output
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
"${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
target_compile_definitions( GXGeometryPlugin PUBLIC
-DQT_NO_DEBUG_OUTPUT )
endif()

# Configure package installation.
target_include_directories( GXGeometryPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
#TODO: unify the Coin/Coin3D name for our Apple/Ubuntu packages!!
source_group( "VP1GeometryPlugin" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GXGeometryPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

install( TARGETS GXGeometryPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GXGeometryPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gxplugins
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GXGeometryPlugin
   COMPONENT Development )
