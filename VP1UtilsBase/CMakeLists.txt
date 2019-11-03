cmake_minimum_required(VERSION 3.13)

set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "GXUtilsBase" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1UtilsBase/*.h )

include_directories ("${PROJECT_SOURCE_DIR}" )
add_library ( GXUtilsBase SHARED ${SOURCES} ${HEADERS}  )

# External dependencies:
find_package( Qt5 COMPONENTS Core )
include_directories(${Qt5Core__INCLUDE_DIRS})
target_link_libraries (GXUtilsBase ${Qt5Core_LIBRARIES})
add_definitions (${Qt5Core_DEFINITIONS})

install(TARGETS GXUtilsBase
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )


set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXUtilsBase PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})



