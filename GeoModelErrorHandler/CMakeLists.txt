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
