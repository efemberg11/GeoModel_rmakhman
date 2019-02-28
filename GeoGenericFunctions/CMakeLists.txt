
# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoGenericFunctions" VERSION 1.0.0 LANGUAGES CXX )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoGenericFunctions/*.h GeoGenericFunctions/*.icc )

# Create the library.
add_library( GeoGenericFunctions SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET GeoGenericFunctions
   PROPERTY PUBLIC_HEADER ${HEADERS} )
target_include_directories( GeoGenericFunctions PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoGenericFunctions" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )

# # Install the library.
# install( TARGETS GeoGenericFunctions
#    EXPORT GeoGenericFunctions
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoGenericFunctions )

# Install a CMake description of the project/library.
# install( EXPORT GeoGenericFunctions DESTINATION cmake )


# new test GeoModelCore
install( TARGETS GeoGenericFunctions EXPORT GeoGenericFunctions-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoGenericFunctions )
