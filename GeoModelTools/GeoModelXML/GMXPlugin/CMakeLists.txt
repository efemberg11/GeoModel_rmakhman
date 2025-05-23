# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Create the library.
add_library( GMXPlugin SHARED ${SOURCES} )
target_link_libraries( GMXPlugin PUBLIC GeoModelCore::GeoModelKernel XercesC::XercesC GeoModelXml )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GMXPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelTools::GMXPlugin ALIAS GMXPlugin )

# Install the library.
install(TARGETS GMXPlugin
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GMXPlugin
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
