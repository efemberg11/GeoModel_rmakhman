# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "MuonDD" VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory(MuonAGDDDescription)
add_subdirectory(MuonGeoModel)
add_subdirectory(MuonAGDDBase)
add_subdirectory(MuonAGDDPlugin)

install(EXPORT MuonAGDDBase-export FILE MuonDD-MuonAGDDBase.cmake DESTINATION lib/cmake/MuonDD)
install(EXPORT MuonGeoModel-export FILE MuonDD-MuonGeoModel.cmake DESTINATION lib/cmake/MuonDD)
install(EXPORT MuonAGDDDescription-export FILE MuonDD-MuonAGDDDescription.cmake DESTINATION lib/cmake/MuonDD)

install(FILES cmake/MuonDDConfig.cmake DESTINATION lib/cmake/MuonDD)

