# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelDataManagers" VERSION 1.0.0 LANGUAGES CXX )

# By default prefer not to use frameworks on macOS.
# But use it at a "LAST" resource, if no other options worked.
# For details on the behaviour of this cache variable, see:
# - https://cmake.org/cmake/help/v3.0/command/find_file.html
set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
   "Framework finding behaviour on macOS" )

add_subdirectory(GeoXmlMatManager)
add_subdirectory(GeoXmlInpManager)

install(EXPORT GeoXmlMatManager-export FILE GeoModelDataManagers-GeoXmlMatManager.cmake DESTINATION lib/cmake/GeoModelDataManagers)
install(EXPORT GeoXmlInpManager-export FILE GeoModelDataManagers-GeoXmlInpManager.cmake DESTINATION lib/cmake/GeoModelDataManagers)

install(FILES cmake/GeoModelDataManagersConfig.cmake DESTINATION lib/cmake/GeoModelDataManagers)
