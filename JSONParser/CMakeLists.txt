# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS JSONParser/*.h )

# Create the library.
add_library( JSONParser SHARED ${HEADERS} ${SOURCES} )
if( nlohmann_json_FOUND )
   target_link_libraries( JSONParser PUBLIC nlohmann_json::nlohmann_json )
else()
   message( WARNING "'nlohmann_json' not found by CMake! The build may fail." )
endif()
target_include_directories( JSONParser PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "JSONParser" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( JSONParser PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS JSONParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS JSONParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/JSONParser
   COMPONENT Development )
