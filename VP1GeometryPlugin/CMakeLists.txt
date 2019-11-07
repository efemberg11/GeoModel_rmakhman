################################################################################
# Package: GXGeometryPlugin
################################################################################
cmake_minimum_required(VERSION 3.12)

set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "GXGeometryPlugin" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()
#find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )

find_package( Coin3D )
find_package( Qt5 COMPONENTS Core Gui Widgets Sql )
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
find_package( Eigen3 REQUIRED )
if( APPLE )
  find_package(OpenGL REQUIRED)
endif()

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


include_directories ("${EIGEN3_INCLUDE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1UtilsBase")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Utils")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1GeometrySystems")
add_library ( GXGeometryPlugin SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )

# External dependencies:
include_directories(${Qt5Core_INCLUDE_DIRS} )
include_directories(${Qt5Gui_INCLUDE_DIRS} )
include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
include_directories(${Qt5Widgets_INCLUDE_DIRS} )
include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
include_directories(${Qt5Network_INCLUDE_DIRS} )
include_directories(${Qt5Sql_INCLUDE_DIRS} )
include_directories(${Coin3D_INCLUDE_DIRS} )
include_directories(${SoQt_INCLUDE_DIRS} )
include_directories(${VP1HEPVis_INCLUDE_DIRS} )
target_link_libraries (GXGeometryPlugin ${Qt5Core_LIBRARIES} GXGeometrySystems GXGui GXBase GXHEPVis GXUtilsBase SoQt Coin Qt5Gui Qt5Core Qt5Widgets Qt5Network Qt5PrintSupport GeoModelKernel GeoGenericFunctions GeoModelRead TFPersistification GeoModelDBManager )
add_definitions (${Qt5Core_DEFINITIONS})

install(TARGETS GXGeometryPlugin
  LIBRARY
  DESTINATION lib/gxplugins
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXGeometryPlugin PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})


