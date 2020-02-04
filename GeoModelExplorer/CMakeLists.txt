# cmake_minimum_required(VERSION 3.12)

# set(MYLIB_VERSION_MAJOR 1)
# set(MYLIB_VERSION_MINOR 0)
# set(MYLIB_VERSION_PATCH 0)
#
# set( CMAKE_BUILD_TYPE DEBUG )
set(CMAKE_CXX_FLAGS "-fPIC -O0 -g -gdwarf-2" )

# project ( "gmex" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( SoQt  )
if ( APPLE )
  find_package( Coin )
else()
  find_package( Coin3D )
endif()


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
# file( GLOB HEADERS RunVP1Light/*.h )



# include_directories ("${PROJECT_SOURCE_DIR}")
# include_directories ("${PROJECT_SOURCE_DIR}/src")
# include_directories ("${PROJECT_SOURCE_DIR}/../GeoModelGUI")

# External dependencies:
# include_directories(${Qt5Core_INCLUDE_DIRS} )
# include_directories(${Qt5Gui_INCLUDE_DIRS} )
# include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
# include_directories(${Qt5Widgets_INCLUDE_DIRS} )
# include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
# include_directories(${Qt5Network_INCLUDE_DIRS} )
#
# include_directories(${Coin3D_INCLUDE_DIRS} )
# include_directories(${SoQt_INCLUDE_DIRS} )

# add_definitions (${Qt5Core_DEFINITIONS})
# add_definitions (${Qt5Gui_DEFINITIONS})
# add_definitions (${Qt5OpenGL_DEFINITIONS})
# add_definitions (${Qt5Widgets_DEFINITIONS})
# add_definitions (${Qt5PrintSupport_DEFINITIONS})
# add_definitions (${Qt5Network_DEFINITIONS})
# message("Qt5Gui_DEFINITIONS: ${Qt5Gui_DEFINITIONS}")


if( APPLE )
   set(CMAKE_MACOSX_RPATH 1)
endif()


add_executable ( gmex ${SOURCES} ${HEADERS}  )
target_link_libraries( gmex VP1GUI VP1Base VP1HEPVis SoQt Coin Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport )

if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
   target_compile_definitions( gmex PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()

install(TARGETS gmex DESTINATION bin)
