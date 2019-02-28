

# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelCore" VERSION 1.0.0 LANGUAGES CXX )


add_subdirectory(GeoGenericFunctions)
add_subdirectory(GeoModelKernel)
add_subdirectory(GeoModelUtilities)

install(EXPORT GeoGenericFunctions-export FILE GeoModelCore-GeoGenericFunctions.cmake DESTINATION lib/GeoModelCore)
install(EXPORT GeoModelKernel-export FILE GeoModelCore-GeoModelKernel.cmake DESTINATION lib/GeoModelCore)
install(EXPORT GeoModelUtilities-export FILE GeoModelCore-GeoModelUtilities.cmake DESTINATION lib/GeoModelCore)
install(FILES cmake/GeoModelCoreConfig.cmake DESTINATION lib/GeoModelCore)
