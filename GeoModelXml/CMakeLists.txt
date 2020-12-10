# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GMX" VERSION 1.0.0 LANGUAGES CXX )

# By default prefer not to use frameworks on macOS.
# But use it at a "LAST" resource, if no other options worked.
# For details on the behaviour of this cache variable, see:
# - https://cmake.org/cmake/help/v3.0/command/find_file.html
set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
   "Framework finding behaviour on macOS" )


add_subdirectory(GeoModelXml)
add_subdirectory(GMXPlugin)

install(EXPORT GeoModelXml-export FILE GMX-GeoModelXml.cmake DESTINATION lib/cmake/GMX)

install(FILES cmake/GMXConfig.cmake DESTINATION lib/cmake/GMX)

