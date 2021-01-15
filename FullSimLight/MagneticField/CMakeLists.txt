# Set up the project.
cmake_minimum_required( VERSION 3.1...3.19 )

project( "MagneticField" VERSION ${GeoModel_VERSION} LANGUAGES CXX )

# Set up the build of the three libraries of the project.
add_subdirectory(MagFieldInterfaces)
add_subdirectory(MagFieldServices)

install(EXPORT MagFieldInterfaces-export FILE MagneticField-MagFieldInterfaces.cmake DESTINATION lib/cmake/MagneticField)
install(EXPORT MagFieldServices-export FILE MagneticField-MagFieldServices.cmake DESTINATION lib/cmake/MagneticField)
install(FILES cmake/MagneticFieldConfig.cmake DESTINATION lib/cmake/MagneticField)
