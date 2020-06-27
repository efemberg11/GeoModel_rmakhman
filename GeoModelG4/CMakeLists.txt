# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelG4" VERSION 1.1.0 LANGUAGES CXX )


# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# External dependencies:
find_package( Geant4 REQUIRED )
# find_package( GeoModelCore 3.2.0 REQUIRED )


# Set up the build of the three libraries of the project.
add_subdirectory(GeoSpecialShapes)
add_subdirectory(GeoMaterial2G4)
add_subdirectory(GeoModel2G4)


install(EXPORT GeoSpecialShapes-export FILE GeoModelG4-GeoSpecialShapes.cmake DESTINATION lib/cmake/GeoModelG4)
install(EXPORT GeoMaterial2G4-export FILE GeoModelG4-GeoMaterial2G4.cmake DESTINATION lib/cmake/GeoModelG4)
install(EXPORT GeoModel2G4-export FILE GeoModelG4-GeoModel2G4.cmake DESTINATION lib/cmake/GeoModelG4)
install(FILES cmake/GeoModelG4Config.cmake DESTINATION lib/cmake/GeoModelG4)
