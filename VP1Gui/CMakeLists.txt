# cmake_minimum_required(VERSION 3.12)

# set(MYLIB_VERSION_MAJOR 1)
# set(MYLIB_VERSION_MINOR 0)
# set(MYLIB_VERSION_PATCH 0)
# set (CMAKE_BUILD_TYPE DEBUG)
set (CMAKE_CXX_FLAGS "-g -O0 -gdwarf-2")
# project ( "VP1GUI" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GUI/*.h )
file( GLOB UIS src/*.ui VP1GUI/*.ui )


# find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )
find_package( SoQt )
if ( APPLE )
  find_package( Coin )
	find_package(OpenGL REQUIRED)
else()
  find_package( Coin3D )
endif()





set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

set (UI_RESOURCES src/vp1.qrc)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/VP1GUI/src)
QT5_WRAP_UI(UI_HDRS ${UIS})
QT5_ADD_RESOURCES(UI_RESOURCES_RCC ${UI_RESOURCES})


# include_directories ("${PROJECT_SOURCE_DIR}")
# include_directories ("${PROJECT_SOURCE_DIR}/src")
# include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
# include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")

# Configure package build
add_library ( VP1GUI SHARED ${SOURCES} ${HEADERS} ${UI_HDRS} ${UI_RESOURCES_RCC}  )
target_link_libraries (VP1GUI VP1Base VP1HEPVis  SoQt Coin Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport )


# Configure package installation
target_include_directories( VP1GUI PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
#TODO: unify the Coin/Coin3D name for our Apple/Ubuntu packages!!
source_group( "VP1GUI" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1GUI PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
   target_compile_definitions( VP1GUI PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()

# Install the library.
install( TARGETS VP1GUI
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS VP1GUI
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1GUI
   COMPONENT Development )


# External dependencies:
# include_directories(${Qt5Core_INCLUDE_DIRS} )
# include_directories(${Qt5Gui_INCLUDE_DIRS} )
# include_directories(${Qt5OpenGL_INCLUDE_DIRS} )
# include_directories(${Qt5Widgets_INCLUDE_DIRS} )
# include_directories(${Qt5PrintSupport_INCLUDE_DIRS} )
# include_directories(${Qt5Network_INCLUDE_DIRS} )
# include_directories(${Coin3D_INCLUDE_DIRS} )
# include_directories(${SoQt_INCLUDE_DIRS} )
# include_directories(${VP1HEPVis_INCLUDE_DIRS} )
# add_definitions (${Qt5Core_DEFINITIONS})

# install(TARGETS VP1GUI
#   LIBRARY
#   DESTINATION lib
#   COMPONENT Libraries
#   )

# set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})
#
# set_target_properties(VP1GUI PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
