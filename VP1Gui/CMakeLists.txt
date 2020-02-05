set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)
set (CMAKE_BUILD_TYPE DEBUG)
set (CMAKE_CXX_FLAGS "-g -O0 -gdwarf-2")

project ( "GXGui" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )



# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1Gui/*.h )
file( GLOB UIS src/*.ui VP1Gui/*.ui )


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

set (UI_RESOURCES src/vp1.qrc)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src)
QT5_WRAP_UI(UI_HDRS ${UIS})
QT5_ADD_RESOURCES(UI_RESOURCES_RCC ${UI_RESOURCES})


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")
add_library ( GXGui SHARED ${SOURCES} ${HEADERS} ${UI_HDRS} ${UI_RESOURCES_RCC}  )

# External dependencies:
include_directories(${Qt5Core_INCLUDE_DIRS} )
include_directories(${Qt5Gui_INCLUDE_DIRS} )
include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
include_directories(${Qt5Widgets_INCLUDE_DIRS} )
include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
#include_directories(${Qt5Network_INCLUDE_DIRS} )
include_directories(${Coin_INCLUDE_DIRS} )
include_directories(${SoQt_INCLUDE_DIRS} )
include_directories(${VP1HEPVis_INCLUDE_DIRS} )
target_link_libraries (GXGui ${Qt5Core_LIBRARIES} GXBase GXHEPVis  SoQt::SoQt Coin::Coin ${Qt5Widgets_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )
add_definitions (${Qt5Core_DEFINITIONS})
install(TARGETS GXGui
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXGui PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
