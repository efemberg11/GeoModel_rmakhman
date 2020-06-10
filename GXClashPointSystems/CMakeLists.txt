# $Id: CMakeLists.txt 732131 2016-03-24 11:03:29Z krasznaa $
################################################################################
# Package: GXClashPointPlugin
################################################################################


set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "-g -O0 -gdwarf-2")

set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)
project ( "GXClashPointSystems" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )

find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )


find_package(nlohmann_json QUIET)
#find_package( Qt5 COMPONENTS Core Gui Widgets Sql )
if( APPLE )
  find_package(OpenGL REQUIRED)
endif()

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GXClashPointSystems/*.h )
file( GLOB UIS src/*.ui GXClashPointSystems/*.ui )


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")

add_library ( GXClashPointSystems SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )

# If we use standalone-build of nlohmann_json parser, add explicit dependency
if( GEOMODEL_USE_BUILTIN_JSON )
   add_dependencies( GXClashPointSystems JSON )
endif()

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
  target_link_directories (GXClashPointSystems PUBLIC ${Coin_LIB_DIR} )
endif()

target_link_libraries (GXClashPointSystems PRIVATE SoQt::SoQt Coin::Coin  ${Qt5Core_LIBRARIES} GXGui GXBase GXHEPVis  ${SoQt_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )

if( GEOMODEL_USE_BUILTIN_JSON )
   add_dependencies( GXClashPointSystems JSON )
endif()

# we link to nlohmann_json only  if we use a version of nlohmann_json installed through CMake;
# this is not needed if the single-header library is installed in a regular
# system include folder (e.g., '/usr/local/include')
if( nlohmann_json_FOUND OR GEOMODEL_USE_BUILTIN_JSON )
  target_link_libraries( GXClashPointSystems PUBLIC nlohmann_json::nlohmann_json )
endif()

add_definitions (${Qt5Core_DEFINITIONS})

install(TARGETS GXClashPointSystems
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXClashPointSystems PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
