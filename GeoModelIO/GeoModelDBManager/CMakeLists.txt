# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
################################################################################
# Package: GeoModelDBManager
# author: Riccardo Maria BIANCHI @ CERN - 2017
# major updates: 
# - R.M.Bianchi, 2018
# - R.M.Bianvhi, Nov 2020
################################################################################

# Find the header and source files.
file( GLOB SOURCES src/*.cpp fmt/*.h )
file( GLOB HEADERS GeoModelDBManager/*.h )

# Set up the library.
add_library( GeoModelDBManager SHARED ${HEADERS} ${SOURCES} )
#if( TARGET SQLite::SQLite3 )
    target_link_libraries( GeoModelDBManager PRIVATE SQLite::SQLite3 )
    #else()
    
    #    target_link_libraries( GeoModelDBManager PRIVATE sqlite3 )
    #endif()
target_include_directories( GeoModelDBManager PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoModelDBManager" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelDBManager PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


# Install the library.
install(TARGETS GeoModelDBManager
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelDBManager
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelDBManager
   COMPONENT Development )
