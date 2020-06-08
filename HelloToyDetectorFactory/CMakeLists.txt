################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloToyDetectorFactory)

# Print Build Info on screen
include(../cmake/PrintBuildInfo.cmake)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore 3.2.0 REQUIRED )
find_package( GeoModelIO 3.2.0 REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager

# Populate a CMake variable with the sources
FILE(GLOB SRCS src/main.cpp src/*.cxx )

# Tell CMake to create the helloworld executable
add_executable( helloToyDetectorFactory ${SRCS} )

# Link all needed libraries
target_link_libraries( helloToyDetectorFactory GeoModelCore::GeoModelKernel GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelWrite Qt5::Core)
