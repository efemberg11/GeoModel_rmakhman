################################################################################
# Package: GeoModelErrorHandler
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoModelErrorHandler" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( Qt5 COMPONENTS Core )

file( GLOB HEADERS GeoModelErrorHandler/*.h )

# Set targets and properties
add_library( GeoModelErrorHandler SHARED ${HEADERS} ${SOURCES} )
target_include_directories( GeoModelErrorHandler PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include>
)
# Set installation of library headers
set_property( TARGET GeoModelErrorHandler PROPERTY PUBLIC_HEADER ${HEADERS} )

# Export as part of GeoModelIO
install( TARGETS GeoModelErrorHandler EXPORT GeoModelErrorHandler-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelErrorHandler )

#
# Version the shared library. (Please update when cutting a new release!)
#
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 3)
set(MYLIB_VERSION_PATCH 0)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GeoModelErrorHandler PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
