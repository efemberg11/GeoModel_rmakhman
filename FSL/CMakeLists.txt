cmake_minimum_required(VERSION 3.5)

project(fsl VERSION 0.1 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )
    # get global GeoModel version
    include( GeoModel-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "fsl" VERSION ${GeoModel_VERSION} LANGUAGES CXX )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Set up how the project handle some of its dependenices. Either by picking them
    # up from the environment, or building them itself.
    if( NOT TARGET JSONExt )
        include( SetupJSON )
    endif()
    # Set a flag to steer the  of the subpackages
    set( ${PROJECT_NAME}_INDIVIDUAL_BUILD ON )
    find_package (FullSimLight REQUIRED)
    add_definitions (-DFSLPLUGINROOT="${FullSimLight_DIR}/../../../${CMAKE_INSTALL_DATAROOTDIR}" )
    message  ( STATUS "FSL plugin files sought here: ${FullSimLight_DIR}/../../../${CMAKE_INSTALL_DATAROOTDIR}" )

else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModel project.")
    # Set the project
    project( "fsl" VERSION ${GeoModel_VERSION} LANGUAGES CXX )
    add_definitions (-DFSLPLUGINROOT="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}" )
endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Support for the HepMC3 exchange format
# This is ON by default, but it can be disabled by the user if not used/needed,
# or if HepMC3 is not installed.
option(GEOMODEL_USE_HEPMC3 "Build GeoModel tools with support for the HepMC3 exchancge format (Note: HepMC3 must be installed on the target machine)" ON)
if(GEOMODEL_USE_HEPMC3)
    find_package(HepMC3 REQUIRED) # required by default, but it can be disabled 
endif()
if(HepMC3_FOUND)
    add_compile_definitions( USE_HEPMC3 )
endif()


# Find the dependencies of the project.
find_package( Qt5 COMPONENTS Core Gui Widgets Sql PrintSupport OpenGL REQUIRED )

# Qt5 settings.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Use the GNU install directory names.
include( GNUInstallDirs )


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB uis ${PROJECT_SOURCE_DIR}/src/*.ui)
set(PROJECT_SOURCES ${sources} ${headers}  ${uis} )


add_executable(fsl ${PROJECT_SOURCES} )


target_link_libraries(fsl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if( GEOMODEL_USE_BUILTIN_JSON )
   add_dependencies(fsl JSONExt )
endif()
 
if( nlohmann_json_FOUND )
    target_link_libraries( fsl PRIVATE nlohmann_json::nlohmann_json )
endif()
  

# Install the executable.
install( TARGETS fsl
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )

