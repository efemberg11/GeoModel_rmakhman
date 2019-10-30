# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelKernel/*.h )

# Create the library.
add_library( GeoModelKernel SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelKernel PUBLIC GeoGenericFunctions )
target_include_directories( GeoModelKernel SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR} )
target_include_directories( GeoModelKernel PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoModelKernel" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
if( GEOMODEL_USE_BUILTIN_EIGEN3 )
   add_dependencies( GeoModelKernel Eigen3 )
endif()
set_target_properties( GeoModelKernel PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS GeoModelKernel
   EXPORT GeoModelCore-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GeoModelKernel
   EXPORT GeoModelCore-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelKernel
   COMPONENT Development )
