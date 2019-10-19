################################################################################
# Package: GeoModelDBManager
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - 2017
# major updates: rbianchi@cern.ch, 2018
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModelDBManager" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( Qt5 COMPONENTS Sql )

# Project's settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs ) # it defines CMAKE_INSTALL_LIBDIR

# comment if you want to get debug messages in Release
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif(CMAKE_BUILD_TYPE MATCHES Release)
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoModelDBManager/*.h )

# Set targets and properties
add_library( GeoModelDBManager SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelDBManager PUBLIC Qt5::Sql )
target_include_directories( GeoModelDBManager SYSTEM PUBLIC ${GeoModelKernel_INCLUDE_DIRS} )
target_include_directories( GeoModelDBManager PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include>
)
# Set installation of library headers
set_property( TARGET GeoModelDBManager PROPERTY PUBLIC_HEADER ${HEADERS} )


###################################################
### Installation ###
###################################################

# Export as part of GeoModelIO
install( TARGETS GeoModelDBManager EXPORT GeoModelDBManager-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelDBManager )
#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 3)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GeoModelDBManager PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
