# Set up the project.
cmake_minimum_required( VERSION 3.1 )

set(CMAKE_CXX_STANDARD 17)

project( "DummyUserActionPlugin" )

#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
    # get global GeoModel version
    #include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "DummyUserActionPlugin" VERSION 1.0 LANGUAGES CXX )
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
    project( "DummyUserActionPlugin" VERSION 1.0 LANGUAGES CXX )
endif()



# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

set(PROJECT_SOURCES ${SOURCES})

# Set up the library.
add_library(DummyUserActionPlugin SHARED ${SOURCES})

#find_package (Eigen3 REQUIRED)
find_package(Geant4 REQUIRED)
#find_package(FullSimLight REQUIRED)
find_package( HDF5 REQUIRED COMPONENTS CXX )

message( STATUS "Found Geant4: ${Geant4_INCLUDE_DIR}")
#message("Geant4_USE_FILE: ${Geant4_USE_FILE}") # debug msg
include(${Geant4_USE_FILE})

# Use the GNU install directory names.
include( GNUInstallDirs )

# Add the FullSimLight headers dir.
# Path is different if built as a single top project, 
# or as an all-in-one build together with all other GeoModel subpackages 
if( FullSimLight_INDIVIDUAL_BUILD )
    target_include_directories( DummyUserActionPlugin PUBLIC  ${CMAKE_SOURCE_DIR} ${HDF5_CXX_INCLUDE_DIRS})
else() # all-in-one--build
    target_include_directories( DummyUserActionPlugin PUBLIC  ${CMAKE_SOURCE_DIR}/FullSimLight ${HDF5_CXX_INCLUDE_DIRS})
endif()

target_link_libraries ( DummyUserActionPlugin PUBLIC ${CMAKE_DL_LIBS} ${Geant4_LIBRARIES} ${HDF5_CXX_LIBRARIES})



set_target_properties( DummyUserActionPlugin PROPERTIES
		       VERSION ${PROJECT_VERSION}
		       SOVERSION ${PROJECT_VERSION_MAJOR} )



install( TARGETS DummyUserActionPlugin
	 LIBRARY DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/FullSimLight/ExamplePlugins/UserActionPlugins
	 COMPONENT Runtime
	 NAMELINK_COMPONENT Development 
	 )



