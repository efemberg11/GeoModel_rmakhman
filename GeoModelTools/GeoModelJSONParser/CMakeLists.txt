# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelJSONParser/*.h )

# Create the library.
add_library( GeoModelJSONParser SHARED ${HEADERS} ${SOURCES} )

# If the in-house build of the nlohmann_json library is used, add explicit dependency
if( GEOMODEL_USE_BUILTIN_JSON )
  add_dependencies( GeoModelJSONParser JSONExt )
  # Acquire Installation Directory of JSONExt
  ExternalProject_Get_Property (JSONExt install_dir)
  # Include the installed 'include' PATH
  include_directories (${install_dir}/include)
endif()

# We link to `nlohmann_json` only  if we use a version of nlohmann_json
# that provides a CMake config file (i.e., either built from source, or also
# installed with Homebrew on macOS).
# This is not needed if the single-header library is installed in a regular
# system include folder (e.g., '/usr/local/include', '/usr/include', ...)
if ( nlohmann_json_FOUND )
  target_link_libraries( GeoModelJSONParser PUBLIC nlohmann_json::nlohmann_json )
endif()


target_include_directories( GeoModelJSONParser PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GeoModelJSONParser" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelJSONParser PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS GeoModelJSONParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GeoModelJSONParser
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelJSONParser
   COMPONENT Development )
