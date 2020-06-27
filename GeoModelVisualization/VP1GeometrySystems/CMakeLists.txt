################################################################################
# Package: VP1GeometryPlugin
################################################################################


set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)
project ( "GXGeometrySystems" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )
find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
# find_package( Eigen3 REQUIRED )
find_package(nlohmann_json QUIET)
if ( APPLE )
  find_package(OpenGL REQUIRED)
endif()

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometrySystems/*.h )
file( GLOB UIS src/*.ui VP1GeometrySystems/*.ui )
file( GLOB DATA share/*.jpg share/*.json )


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

# If we use standalone-build of nlohmann_json parser, add explicit dependency
if( GEOMODEL_USE_BUILTIN_JSON )
   add_dependencies( GXGeometrySystems JSON )
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


# we link to nlohmann_json only  if we use a version of nlohmann_json installed through CMake;
# this is not needed if the single-header library is installed in a regular
# system include folder (e.g., '/usr/local/include')
if( nlohmann_json_FOUND OR GEOMODEL_USE_BUILTIN_JSON )
  target_link_libraries( GXGeometrySystems nlohmann_json::nlohmann_json )
endif()

if ( APPLE )
  target_link_directories (GXGeometrySystems PUBLIC ${Coin_LIB_DIR} )
  target_link_libraries (GXGeometrySystems SoQt::SoQt Coin::Coin GeoModelKernel ${Qt5Core_LIBRARIES} GXGui GXBase GXHEPVis GeoModelRead GeoModelWrite ${SoQt_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )
  else()
  target_link_libraries (GXGeometrySystems SoQt::SoQt Coin::Coin GeoModelKernel ${Qt5Core_LIBRARIES} GXGui GXBase GXHEPVis GeoModelRead GeoModelWrite ${SoQt_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )
endif()


# Set a variable storing the CMake install prefix.
# -- It is passed to the C++ preprocessor to load, from the C++ code,
# -- the JSON files storing the default materials and volume attributes
# -- for Coin3D.
add_definitions ( -DGEOMODEL_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})



add_definitions (${Qt5Core_DEFINITIONS})

install(TARGETS GXGeometrySystems
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )
install( FILES ${DATA} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/gmex)

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXGeometrySystems PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
