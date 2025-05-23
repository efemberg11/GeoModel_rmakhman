# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelValidation/*.h )

#find_package( GeoModelUtilities REQUIRED )

# Create the library.
add_library( GeoModelValidation SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelValidation  PUBLIC Eigen3::Eigen GeoModelCore::GeoGenericFunctions GeoModelCore::GeoModelKernel 
                                                                GeoModelCore::GeoModelHelpers GeoModelIO::GeoModelDBManager 
                                                                GeoModelIO::GeoModelWrite GeoModelIO::GeoModelRead ) 
target_include_directories( GeoModelValidation PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GeoModelValidation" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelValidation PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelTools::GeoModelValidation ALIAS GeoModelValidation )

# Install the library.
install(TARGETS GeoModelValidation
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelValidation
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelValidation
   COMPONENT Development )
