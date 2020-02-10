cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "JSONParser" VERSION 1.0.0 LANGUAGES CXX )

find_package( GeoModelCore  REQUIRED )
find_package( nlohmann_json QUIET )
find_package( Eigen3 REQUIRED )


# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS JSONParser/*.h )

# Create the library.
add_library( JSONParser SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET JSONParser
   PROPERTY PUBLIC_HEADER ${HEADERS} )
if( nlohmann_json_FOUND )
  target_link_libraries( JSONParser PUBLIC nlohmann_json::nlohmann_json GeoModelCore::GeoModelKernel )
else()
  message(STATUS "'nlohmann_json' not found by CMake!! Anyway, if you installed the single header file in a standard system include dir, I will be able to use it.")
  target_link_libraries( JSONParser PUBLIC GeoModelCore::GeoModelKernel )
endif()
target_include_directories( JSONParser SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR} )
target_include_directories( JSONParser PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "JSONParser" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )

install( TARGETS JSONParser EXPORT JSONParser-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/JSONParser )
#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(JSONParser PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
