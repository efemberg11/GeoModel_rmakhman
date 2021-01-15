# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelKernel/*.h GeoModelKernel/*.tpp )

# Create the library.
add_library( GeoModelKernel SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelKernel PUBLIC Eigen3::Eigen GeoGenericFunctions
   ${CMAKE_DL_LIBS} )
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

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelCore::GeoModelKernel ALIAS GeoModelKernel )

# Install the library.
install(TARGETS GeoModelKernel
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelKernel
   COMPONENT Development )
