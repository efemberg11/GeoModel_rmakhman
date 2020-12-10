# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "AGDD" VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory(AGDDKernel)
add_subdirectory(AGDDModel)
add_subdirectory(AGDDControl)
add_subdirectory(AGDDHandlers)
add_subdirectory(AGDDPlugin)

install(EXPORT AGDDKernel-export FILE AGDD-AGDDKernel.cmake DESTINATION lib/cmake/AGDD)
install(EXPORT AGDDModel-export FILE AGDD-AGDDModel.cmake DESTINATION lib/cmake/AGDD)
install(EXPORT AGDDControl-export FILE AGDD-AGDDControl.cmake DESTINATION lib/cmake/AGDD)
install(EXPORT AGDDHandlers-export FILE AGDD-AGDDHandlers.cmake DESTINATION lib/cmake/AGDD)

install(FILES cmake/AGDDConfig.cmake DESTINATION lib/cmake/AGDD)

