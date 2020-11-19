# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
################################################################################
# Package: GeoModelDBManager
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - 2017
# major updates: rbianchi@cern.ch, 2018
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
install( TARGETS GeoModelDBManager
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     NAMELINK_COMPONENT Development
   NAMELINK_ONLY
     COMPONENT Development )
#install( TARGETS GeoModelDBManager
#   EXPORT ${PROJECT_NAME}-export
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   COMPONENT Development
#   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelDBManager
   COMPONENT Development )
