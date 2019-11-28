################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloToyDetectorFactory)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Sql Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager

# Populate a CMake variable with the sources
FILE(GLOB SRCS src/main.cpp src/*.cxx )

# Tell CMake to create the helloworld executable
add_executable( helloToyDetectorFactory ${SRCS} )

# Link all needed libraries
target_link_libraries( helloToyDetectorFactory GeoModelCore::GeoModelKernel GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelWrite Qt5::Core Qt5::Sql)
