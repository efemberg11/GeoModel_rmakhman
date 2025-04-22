# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelXMLDumper/*.h GeoModelXMLDumper/*.icc )

# Create the library.
add_library( GeoModelXMLDumper SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelXMLDumper PUBLIC GeoModelCore::GeoModelKernel 
                                                GeoModelCore::GeoModelHelpers )
target_include_directories( GeoModelXMLDumper PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GeoModelXMLDumper" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelXMLDumper PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelTools::GeoModelXMLDumper ALIAS GeoModelXMLDumper )

# Install the library.
install(TARGETS GeoModelXMLDumper
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelXMLDumper
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelXMLDumper
   COMPONENT Development )



# Declare the package's executable.
add_executable( dumpGeoXML util/main.cxx)
target_link_libraries( dumpGeoXML PRIVATE GeoModelCore::GeoModelKernel
                                          GeoModelCore::GeoModelHelpers
                                          GeoModelIO::GeoModelRead
                                          GeoModelTools::GeoModelXMLDumper)

# Tweak how debug information should be attached to the executable, in Debug
# builds.
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND
   "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )
   target_compile_options( dumpGeoXML PRIVATE "-gdwarf-2" )
endif()

# Install the executable.
install( TARGETS dumpGeoXML
   EXPORT ${PROJECT_NAME}-export
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )
