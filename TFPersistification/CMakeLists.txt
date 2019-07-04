################################################################################
# Package: TFPersistification
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "TFPersistification" VERSION 1.0.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS TFPersistification/*.h )

# Dependencies.
find_package( GeoModelCore REQUIRED )

# Find an existing installation of Eigen3.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
find_package( Eigen3 REQUIRED )


# Create the library.
add_library( TFPersistification SHARED ${HEADERS} ${SOURCES} )
set_property( TARGET TFPersistification
   PROPERTY PUBLIC_HEADER ${HEADERS} )

if( BUILTIN_EIGEN3 )
   add_dependencies( TFPersistification EIGEN3 )
endif()

target_include_directories ( TFPersistification SYSTEM PUBLIC ${GeoModelCore_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} )

target_include_directories( TFPersistification PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

target_link_libraries( TFPersistification PUBLIC GeoModelKernel )

# Export as part of GeoModelIO
install( TARGETS TFPersistification EXPORT TFPersistification-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TFPersistification )
