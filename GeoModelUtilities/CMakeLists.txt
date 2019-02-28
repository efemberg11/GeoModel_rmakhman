################################################################################
# Package: GeoModelUtilities
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModelUtilities" VERSION 1.0.0 LANGUAGES CXX )

# Project's settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs ) # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelUtilities/*.h )

# Set targets and properties
add_library( GeoModelUtilities SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelUtilities PUBLIC GeoModelKernel )
target_include_directories( GeoModelUtilities SYSTEM PUBLIC ${GeoModelKernel_INCLUDE_DIRS} )
target_include_directories( GeoModelUtilities PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include>
)

# Set installation of library headers
set_property( TARGET GeoModelUtilities PROPERTY PUBLIC_HEADER ${HEADERS} )

# # Install the library and set export target
# install( TARGETS GeoModelUtilities
#    EXPORT GeoModelUtilitiesConfig
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelUtilities
#  )
#
# # Install a CMake description of the project/library.
# install( EXPORT GeoModelUtilitiesConfig DESTINATION cmake )

# new test GeoModelCore
install( TARGETS GeoModelUtilities EXPORT GeoModelUtilities-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelUtilities )
