# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GMX" VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory(GeoModelXml)
add_subdirectory(GMXPlugin)

install(EXPORT GeoModelXml-export FILE GMX-GeoModelXml.cmake DESTINATION lib/cmake/GMX)

install(FILES cmake/GMXConfig.cmake DESTINATION lib/cmake/GMX)

