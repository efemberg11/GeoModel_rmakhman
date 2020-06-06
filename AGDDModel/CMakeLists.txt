cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "AGDDModel" VERSION 1.0.0 LANGUAGES CXX )

find_package( GeoModelCore REQUIRED )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS AGDDModel/*.h )

# Create the library.
add_library( AGDDModel SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET AGDDModel
   PROPERTY PUBLIC_HEADER ${HEADERS} )
target_link_libraries( AGDDModel PUBLIC AGDDKernel GeoModelCore::GeoModelKernel )
target_include_directories( AGDDModel SYSTEM PUBLIC )
target_include_directories( AGDDModel PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "AGDDModel" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )

# new test GeoModelCore
install( TARGETS AGDDModel EXPORT AGDDModel-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/AGDDModel )
#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(AGDDModel PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})

