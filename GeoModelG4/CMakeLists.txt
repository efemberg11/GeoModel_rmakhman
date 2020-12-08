# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# CMake settings
cmake_minimum_required( VERSION 3.14 )

# Dummy call to 'project()', needed to set 'PROJECT_SOURCE_DIR'
project( "GeoModelG4" )
#Set up the project. Check if we build it with GeoModel or individually
if( (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) OR GEOMODEL_BUILD_GEOMODELG4_FROM_FULLSIMLIGHT )
    # I am top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )
    # get global GeoModel version
    include( GeoModel-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "GeoModelG4" VERSION ${GeoModel_VERSION} LANGUAGES CXX )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Find the base GeoModel packages, which must be installed on the target system already
    find_package( GeoModelCore REQUIRED ${GeoModel_VERSION} ) 
    # Find Geant4, if not triggered by FullSimLight, where Geant4 is already found/set.
    if( NOT GEOMODEL_BUILD_GEOMODELG4_FROM_FULLSIMLIGHT )
        find_package( Geant4 REQUIRED )
        include( SetupXercesC )
    endif()
    # Set a flag to steer the  of the subpackages
    set( ${PROJECT_NAME}_INDIVIDUAL_BUILD ON )
else()
    # I am called from other projects with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModel project.")
    # Set the project
    project( "GeoModelG4" VERSION ${GeoModel_VERSION} LANGUAGES CXX )
    # External dependencies:
    find_package( Geant4 REQUIRED )
endif()

# Set up the build of the three libraries of the project.
add_subdirectory(GeoSpecialShapes)
add_subdirectory(GeoMaterial2G4)
add_subdirectory(GeoModel2G4)


install(EXPORT GeoSpecialShapes-export FILE GeoModelG4-GeoSpecialShapes.cmake DESTINATION lib/cmake/GeoModelG4)
install(EXPORT GeoMaterial2G4-export FILE GeoModelG4-GeoMaterial2G4.cmake DESTINATION lib/cmake/GeoModelG4)
install(EXPORT GeoModel2G4-export FILE GeoModelG4-GeoModel2G4.cmake DESTINATION lib/cmake/GeoModelG4)
install(FILES cmake/GeoModelG4Config.cmake DESTINATION lib/cmake/GeoModelG4)
