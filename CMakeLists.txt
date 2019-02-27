

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelIO" VERSION 1.0.0 LANGUAGES CXX )

# Project's dependencies.
find_package( GeoModelCore REQUIRED )

# Set sub-packages to build.
# add_subdirectory(GeoModelErrorHandler)
add_subdirectory(GeoModelDBManager)
add_subdirectory(TFPersistification)
add_subdirectory(GeoModelRead)
add_subdirectory(GeoModelWrite)

# Set the export of all the sub-packages' 
# install(EXPORT GeoModelErrorHandler-export FILE GeoModelIO-GeoModelErrorHandler.cmake DESTINATION lib/GeoModelIO)
install(EXPORT GeoModelDBManager-export FILE GeoModelIO-GeoModelDBManager.cmake DESTINATION lib/GeoModelIO)
install(EXPORT TFPersistification-export FILE GeoModelIO-TFPersistification.cmake DESTINATION lib/GeoModelIO)
install(EXPORT GeoModelRead-export FILE GeoModelIO-GeoModelRead.cmake DESTINATION lib/GeoModelIO)
install(EXPORT GeoModelWrite-export FILE GeoModelIO-GeoModelWrite.cmake DESTINATION lib/GeoModelIO)
install(FILES cmake/GeoModelIOConfig.cmake DESTINATION lib/GeoModelIO)
