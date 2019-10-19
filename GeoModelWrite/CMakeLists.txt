################################################################################
# Package: GeoModelWrite
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModelWrite" VERSION 1.0.0 LANGUAGES CXX )

# Project's settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs ) # it defines CMAKE_INSTALL_LIBDIR

# Dependencies.
## Third-party dependencies.
find_package( Qt5 COMPONENTS Core Sql) # for Qt containers # TODO: remove Sql dependency, which is brought by GeoModelDBManager
## GeoModelIO dependencies.
find_package( GeoModelCore REQUIRED )


if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DQT_NO_DEBUG_OUTPUT) # comment if you need debug messages in Release
endif(CMAKE_BUILD_TYPE MATCHES Release)
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    add_definitions(-DQT_NO_DEBUG_OUTPUT) # comment if you need debug messages in RelWithDebInfo
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)


# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoModelWrite/*.h )

# Set targets and properties
add_library( GeoModelWrite SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelWrite PUBLIC Qt5::Core GeoModelKernel GeoModelDBManager TFPersistification )
target_include_directories( GeoModelWrite SYSTEM PUBLIC ${GeoModelCore_INCLUDE_DIRS} ${GeoModelDBManager_INCLUDE_DIRS} )
target_include_directories( GeoModelWrite PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include>
)

# Set installation of library headers
set_property( TARGET GeoModelWrite PROPERTY PUBLIC_HEADER ${HEADERS} )

# Install the library and set export target
install( TARGETS GeoModelWrite
   EXPORT GeoModelWriteConfig
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelWrite
 )

# Install a CMake description of the project/library.
install( EXPORT GeoModelWriteConfig DESTINATION cmake )

# Export as part of GeoModelIO
install( TARGETS GeoModelWrite EXPORT GeoModelWrite-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelWrite )

#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 3)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GeoModelWrite PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
