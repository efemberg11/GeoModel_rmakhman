
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "GXHEPVis" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin REQUIRED )
find_package( OpenGL REQUIRED )


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1HEPVis/*.h )


include_directories ("${PROJECT_SOURCE_DIR}")

add_library ( GXHEPVis SHARED ${SOURCES} ${HEADERS}  )

# Using find_package(GeoModelCore) is only needed when building GeoModelVisualization individually,
# i.e., after having already compiled the base packages (GeoModelCore). 
# This is used when building distribution packages. 
find_package( GeoModelCore ${GeoModelCore_VERSION} QUIET )
if( GeoModelCore_FOUND ) # if built individually
    target_link_libraries( GXHEPVis PUBLIC GeoModelCore::GeoModelKernel )
else() # if built as part of the build of GeoModelCore and the other base libraries
    target_link_libraries( GXHEPVis PUBLIC GeoModelKernel )
endif()
target_link_libraries( GXHEPVis PUBLIC Coin::Coin )
if ( APPLE )
  target_link_libraries (GXHEPVis PUBLIC ${OPENGL_LIBRARIES} )
endif()


install(TARGETS GXHEPVis
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXHEPVis PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
