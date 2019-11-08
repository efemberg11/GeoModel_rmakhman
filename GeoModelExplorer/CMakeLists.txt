cmake_minimum_required(VERSION 3.12)

set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

set( CMAKE_BUILD_TYPE RELEASE )
#set(CMAKE_CXX_FLAGS "-fPIC -O0 -g -gdwarf-2" )

project ( "gmex" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin3D  )
find_package( SoQt  )
if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )



# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS RunVP1Light/*.h )



include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Gui")
add_executable ( gmex ${SOURCES} ${HEADERS}  )

# External dependencies:
include_directories(${Qt5Core_INCLUDE_DIRS} )
include_directories(${Qt5Gui_INCLUDE_DIRS} )
include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
include_directories(${Qt5Widgets_INCLUDE_DIRS} )
include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
include_directories(${Qt5Network_INCLUDE_DIRS} )

include_directories(${Coin3D_INCLUDE_DIRS} )
include_directories(${SoQt_INCLUDE_DIRS} )

add_definitions (${Qt5Core_DEFINITIONS})
add_definitions (${Qt5Gui_DEFINITIONS})
add_definitions (${Qt5OpenGL_DEFINITIONS})
add_definitions (${Qt5Widgets_DEFINITIONS})
add_definitions (${Qt5PrintSupport_DEFINITIONS})
add_definitions (${Qt5Network_DEFINITIONS})
target_link_libraries( gmex GXGui GXBase GXHEPVis SoQt Coin Qt5Gui Qt5Core Qt5Widgets Qt5Network Qt5PrintSupport )
install(TARGETS gmex DESTINATION bin)



