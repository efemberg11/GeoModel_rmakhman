# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
################################################################################
# Package: GeoModelRead
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - 2017
# major updates: rbianchi@cern.ch, 2018
################################################################################

# Set up configuration variables for turning certain build flags on or off.
option( GEOMODEL_IO_READ_DEBUG
   "Turn on debug level output from the GeoModel I/O code by default" FALSE )
option( GEOMODEL_IO_DEBUG_VERBOSE
   "Turn on verbose level output from the GeoModel I/O code by default" FALSE )
option( GEOMODEL_IO_READ_TIMING
   "Turn on timing measurements in the GeoModel I/O code by default" FALSE )

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoModelRead/*.h GeoModelRead/*.tpp )

# Set up the library.
add_library( GeoModelRead SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelRead PUBLIC
GeoModelKernel GeoModelDBManager TFPersistification )
target_include_directories( GeoModelRead PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoModelRead" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelRead PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up custom build flags for the library.
foreach( _flag GEOMODEL_IO_READ_DEBUG GEOMODEL_IO_DEBUG_VERBOSE
   GEOMODEL_IO_READ_TIMING )
   if( ${${_flag}} )
      target_compile_definitions( GeoModelRead PRIVATE ${_flag}=true )
   endif()
endforeach()

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelIO::GeoModelRead ALIAS GeoModelRead )

# Install the library.
install(TARGETS GeoModelRead
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelRead
   COMPONENT Development )
