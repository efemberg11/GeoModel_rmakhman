################################################################################
# Package: GeoModelRead
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - 2017
# major updates: rbianchi@cern.ch, 2018
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModelRead" VERSION 1.0.0 LANGUAGES CXX )

### Dependencies.
# Third-party dependencies.
find_package( Qt5 COMPONENTS Core Sql ) # for QDebug # TODO: remove Sql dependency, which is brought by GeoModelDBManager
# GeoModelIO dependencies.
find_package( GeoModelCore REQUIRED )

# Project's settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs ) # it defines CMAKE_INSTALL_LIBDIR

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DQT_NO_DEBUG_OUTPUT) # comment out if you want to get debug messages in Release
endif(CMAKE_BUILD_TYPE MATCHES Release)
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    add_definitions(-DQT_NO_DEBUG_OUTPUT) # comment out if you want to get debug messages in RelWithDebInfo
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoModelRead/*.h )

# Set targets and properties
add_library( GeoModelRead SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelRead PUBLIC Qt5::Core GeoModelKernel GeoModelDBManager TFPersistification )
target_include_directories( GeoModelRead SYSTEM PUBLIC ${GeoModelCore_INCLUDE_DIRS} ${GeoModelDBManager_INCLUDE_DIRS} )
target_include_directories( GeoModelRead PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include>
)

# Set installation of library headers
set_property( TARGET GeoModelRead PROPERTY PUBLIC_HEADER ${HEADERS} )

# Install the library and set export target
install( TARGETS GeoModelRead
   EXPORT GeoModelReadConfig
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelRead
 )

# Install a CMake description of the project/library.
install( EXPORT GeoModelReadConfig DESTINATION cmake )

# Export as part of GeoModelIO
install( TARGETS GeoModelRead EXPORT GeoModelRead-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelRead )

#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 3)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GeoModelRead PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
