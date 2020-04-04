cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "AGDDHandlers" VERSION 1.0.0 LANGUAGES CXX )

find_package( GeoModelCore REQUIRED )
find_package( CLHEP REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( XercesC REQUIRED )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS AGDDHandlers/*.h )

# Create the library.
add_library( AGDDHandlers SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET AGDDHandlers
   PROPERTY PUBLIC_HEADER ${HEADERS} )
target_link_libraries( AGDDHandlers PUBLIC AGDDKernel AGDDModel GeoModelXMLParser GeoModelCore::GeoModelKernel ExpressionEvaluator XercesC::XercesC CLHEP::CLHEP )
target_include_directories( AGDDHandlers SYSTEM PUBLIC )
target_include_directories( AGDDHandlers PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "AGDDHandlers" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )

# new test GeoModelCore
install( TARGETS AGDDHandlers EXPORT AGDDHandlers-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/AGDDHandlers )
#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(AGDDHandlers PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})

