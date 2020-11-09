# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelG4" VERSION 1.1.0 LANGUAGES CXX )

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
if( (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) OR GEOMODEL_BUILD_GEOMODELG4 )
    # I am top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Set up how the project handle some of its dependenices. Either by picking them
    # up from the environment, or building them itself.^[[?12;4$y
    include( SetupJSON )
    # Find the base GeoModel packages, which must be installed on the target system already
    find_package( GeoModelCore REQUIRED 4.0.0 ) #TODO: the version should be se in the root folder
    # Set a flag to steer the  of the subpackages
    set( ${PROJECT_NAME}_INDIVIDUAL_BUILD ON )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModel project.")
endif()
# External dependencies:
find_package( Geant4 REQUIRED )

# Set up the build of the three libraries of the project.
add_subdirectory(GeoSpecialShapes)
add_subdirectory(GeoMaterial2G4)
add_subdirectory(GeoModel2G4)


install(EXPORT GeoSpecialShapes-export FILE GeoModelG4-GeoSpecialShapes.cmake DESTINATION lib/cmake/GeoModelG4)
install(EXPORT GeoMaterial2G4-export FILE GeoModelG4-GeoMaterial2G4.cmake DESTINATION lib/cmake/GeoModelG4)
install(EXPORT GeoModel2G4-export FILE GeoModelG4-GeoModel2G4.cmake DESTINATION lib/cmake/GeoModelG4)
install(FILES cmake/GeoModelG4Config.cmake DESTINATION lib/cmake/GeoModelG4)
