cmake_minimum_required(VERSION 3.12)
set( CMAKE_CXX_FLAGS "-O0" )
project( "GeoModelPlugins-Test" LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

find_package( GeoModelCore REQUIRED )
find_package( GeoModelDataManagers REQUIRED )

add_executable ( toytest src/toytest.cxx )
target_link_libraries ( toytest GeoModelCore::GeoModelKernel GeoModelDataManagers::GeoXmlMatManager )

install( TARGETS toytest DESTINATION bin )
