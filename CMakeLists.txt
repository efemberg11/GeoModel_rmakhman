
cmake_minimum_required( VERSION 3.1 )

# Set up the project.
project( "GeoModelG4" VERSION 1.0.0 LANGUAGES CXX )

# Add sub-packages.
add_subdirectory(GeoMaterial2G4)
add_subdirectory(GeoModel2G4)
add_subdirectory(GeoSpecialShapes)

install(EXPORT GeoMaterial2G4-export FILE GeoModelG4-GeoMaterial2G4.cmake DESTINATION lib/GeoModelG4)
install(EXPORT GeoModel2G4-export FILE GeoModelG4-GeoModel2G4.cmake DESTINATION lib/GeoModelG4)
install(EXPORT GeoSpecialShapes-export FILE GeoModelG4-GeoSpecialShapes.cmake DESTINATION lib/GeoModelG4)
install(FILES cmake/GeoModelG4Config.cmake DESTINATION lib/GeoModelG4)
