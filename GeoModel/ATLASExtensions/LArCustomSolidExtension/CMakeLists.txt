# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration

cmake_minimum_required( VERSION 3.12 )
project( "LArCustomShapeExtensionSolid" )


#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
    # get global GeoModel version
    #include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "LArCustomShapeExtensionSolid" VERSION 1.0 LANGUAGES CXX )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Use the GNU install directory names.
    include( GNUInstallDirs )
    # Set a default build type
    include( BuildType )
    # Set default build and C++ options
    include( configure_cpp_options )
    # Print Build Info on screen
    include( PrintBuildInfo )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # GeoModel dependencies
    find_package( GeoModelCore REQUIRED )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root project.")
    # Set the project
    project( "LArCustomShapeExtensionSolid" VERSION 1.0 LANGUAGES CXX )
endif()

# Other project's dependencies.

find_package( Geant4 REQUIRED )     

include(${Geant4_USE_FILE})

# Find the header and source files.
file( GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/import/DetectorDescription/GeoModel/GeoSpecialShapes/src/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/import/DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/import/Simulation/G4Utilities/Geo2G4/src/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/LArCustomShapeExtensionSolid/*.cxx)

# Define include directories
include_directories(  ${CMAKE_CURRENT_SOURCE_DIR}/import/Control/CxxUtils ${CMAKE_CURRENT_SOURCE_DIR}/import/DetectorDescription/GeoModel/GeoSpecialShapes ${CMAKE_CURRENT_SOURCE_DIR}/import/DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl  ${CMAKE_CURRENT_SOURCE_DIR}/import/Simulation/G4Utilities/Geo2G4 ${CMAKE_CURRENT_SOURCE_DIR}/import/Simulation/G4Utilities/Geo2G4/src ${GEANT4_INCLUDE_DIRS}  )

add_definitions ( -DPORTABLE_LAR_SHAPE )

add_library( LArCustomShapeExtensionSolid SHARED ${SOURCES} )
target_link_libraries( LArCustomShapeExtensionSolid PUBLIC GeoModelCore::GeoModelKernel ${Geant4_LIBRARIES} ) 
set_target_properties( LArCustomShapeExtensionSolid PROPERTIES
		       VERSION ${PROJECT_VERSION}
		       SOVERSION ${PROJECT_VERSION_MAJOR} )

install( TARGETS LArCustomShapeExtensionSolid
	 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	 COMPONENT Runtime
	 NAMELINK_COMPONENT Development )
