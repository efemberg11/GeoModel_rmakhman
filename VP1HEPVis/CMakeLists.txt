
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "GXHEPVis" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin REQUIRED )
# if ( APPLE )
	find_package(OpenGL REQUIRED)
# endif()



# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1HEPVis/*.h )




include_directories ("${PROJECT_SOURCE_DIR}")
add_library ( GXHEPVis SHARED ${SOURCES} ${HEADERS}  )

# External dependencies:
find_package( Qt5 COMPONENTS Core )
include_directories(${Qt5Core__INCLUDE_DIRS} )
include_directories(${Coin_INCLUDE_DIR} )
if ( APPLE )
  target_link_libraries (GXHEPVis ${Qt5Core_LIBRARIES} ${Coin_LIBRARIES} ${OPENGL_LIBRARIES} )
  target_link_directories (GXHEPVis PUBLIC ${Coin_LIB_DIR} )
else()
  target_link_libraries (GXHEPVis ${Qt5Core_LIBRARIES} )
endif()
add_definitions (${Qt5Core_DEFINITIONS})


install(TARGETS GXHEPVis
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXHEPVis PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
