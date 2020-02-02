
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

set( CMAKE_BUILD_TYPE DEBUG )
set(CMAKE_CXX_FLAGS "-fPIC -O0 -g -gdwarf-2" )

project ( "gmcat" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
find_package( Eigen3 REQUIRED )
file( GLOB SOURCES src/*.cxx )

include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
add_executable ( gmcat ${SOURCES} ${HEADERS}  )

# External dependencies:

target_link_libraries (gmcat GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
install(TARGETS gmcat DESTINATION bin)




