# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoGenericFunctions/*.h GeoGenericFunctions/*.icc )

# Create the library.
add_library( GeoGenericFunctions SHARED ${HEADERS} ${SOURCES} )
target_include_directories( GeoGenericFunctions PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoGenericFunctions" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoGenericFunctions PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install(TARGETS GeoGenericFunctions
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoGenericFunctions
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
#install( TARGETS GeoGenericFunctions
#   EXPORT ${PROJECT_NAME}-export
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     COMPONENT Runtime
#     NAMELINK_COMPONENT Development
#   NAMELINK_ONLY 
#     COMPONENT Development )
#install( TARGETS GeoGenericFunctions
#   EXPORT ${PROJECT_NAME}-export
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   COMPONENT Development
#   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoGenericFunctions
   COMPONENT Development )
