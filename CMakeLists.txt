# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelDataManagers" VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory(GeoXmlMatManager)
add_subdirectory(GeoXmlInpManager)

install(EXPORT GeoXmlMatManager-export FILE GeoModelDataManagers-GeoXmlMatManager.cmake DESTINATION lib/cmake/GeoModelDataManagers)
install(EXPORT GeoXmlInpManager-export FILE GeoModelDataManagers-GeoXmlInpManager.cmake DESTINATION lib/cmake/GeoModelDataManagers)

install(FILES cmake/GeoModelDataManagersConfig.cmake DESTINATION lib/cmake/GeoModelDataManagers)
