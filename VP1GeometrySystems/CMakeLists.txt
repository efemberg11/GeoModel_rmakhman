# $Id: CMakeLists.txt 732131 2016-03-24 11:03:29Z krasznaa $
################################################################################
# Package: VP1GeometryPlugin
################################################################################


set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "-g -O0 -gdwarf-2")

set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)
project ( "GXGeometrySystems" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )

find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
#find_package( Qt5 COMPONENTS Core Gui Widgets Sql )
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
find_package( Eigen3 REQUIRED )
if( APPLE )
  find_package(OpenGL REQUIRED)
endif()

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometrySystems/*.h )
file( GLOB UIS src/*.ui VP1GeometrySystems/*.ui )


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
add_library ( GXGeometrySystems SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )

# External dependencies:
include_directories(${Qt5Core_INCLUDE_DIRS} )
include_directories(${Qt5Gui_INCLUDE_DIRS} )
include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
include_directories(${Qt5Widgets_INCLUDE_DIRS} )
include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
include_directories(${Qt5Sql_INCLUDE_DIRS} )
include_directories(${Coin_INCLUDE_DIR} )
include_directories(${SoQt_INCLUDE_DIRS} )
include_directories(${VP1HEPVis_INCLUDE_DIRS} )

if ( APPLE )
  target_link_directories (GXGeometrySystems PUBLIC ${Coin_LIB_DIR} )
  target_link_libraries (GXGeometrySystems ${SoQt_LIBRARIES} ${Coin_LIBRARIES} GeoModelCore::GeoModelKernel ${Qt5Core_LIBRARIES} GXGui GXBase GXHEPVis GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite ${SoQt_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )
  else()
  target_link_libraries (GXGeometrySystems ${SoQt_LIBRARIES} ${Coin_LIBRARIES} GeoModelCore::GeoModelKernel ${Qt5Core_LIBRARIES} GXGui GXBase GXHEPVis GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite ${SoQt_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )
endif()



add_definitions (${Qt5Core_DEFINITIONS})

install(TARGETS GXGeometrySystems
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXGeometrySystems PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
