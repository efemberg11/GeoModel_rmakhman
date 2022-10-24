# Set up the project.
cmake_minimum_required( VERSION 3.1 )

set(CMAKE_CXX_STANDARD 17)

project( "TestPhysicsListPlugin" )

#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
    # get global GeoModel version
    #include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "TestPhysicsListPlugin" VERSION 1.0 LANGUAGES CXX )
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
    project( "TestPhysicsListPlugin" VERSION 1.0 LANGUAGES CXX )
endif()



# Find the header and source files.
file( GLOB SOURCES src/*.cc )

set(PROJECT_SOURCES ${SOURCES})

# Set up the library.
add_library(TestPhysicsListPlugin SHARED ${SOURCES})

find_package(Geant4 REQUIRED)

message( STATUS "Found Geant4: ${Geant4_INCLUDE_DIR}")
#message("Geant4_USE_FILE: ${Geant4_USE_FILE}") # debug msg
include(${Geant4_USE_FILE})

# Use the GNU install directory names.
include( GNUInstallDirs )

# Add the FullSimLight headers dir.
# Path is different if built as a single top project, 
# or as an all-in-one build together with all other GeoModel subpackages 
if( FullSimLight_INDIVIDUAL_BUILD )
    target_include_directories( TestPhysicsListPlugin PUBLIC  ${CMAKE_SOURCE_DIR} )
else() # all-in-one--build
    target_include_directories( TestPhysicsListPlugin PUBLIC  ${CMAKE_SOURCE_DIR}/FullSimLight )
endif()

target_link_libraries ( TestPhysicsListPlugin PUBLIC ${CMAKE_DL_LIBS} ${Geant4_LIBRARIES})



set_target_properties( TestPhysicsListPlugin PROPERTIES
		       VERSION ${PROJECT_VERSION}
		       SOVERSION ${PROJECT_VERSION_MAJOR} )



install( TARGETS TestPhysicsListPlugin
	 LIBRARY DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/FullSimLight/PhysicsListPlugins
	 COMPONENT Runtime
	 NAMELINK_COMPONENT Development 
	 )



