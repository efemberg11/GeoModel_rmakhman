################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloToyDetectorFactory)

# Compile with C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )

# Populate a CMake variable with the sources
FILE(GLOB SRCS src/main.cpp src/*.cxx )

# Tell CMake to create the helloworld executable
add_executable( helloToyDetectorFactory ${SRCS} )

# Link all needed libraries
target_link_libraries( helloToyDetectorFactory GeoModelKernel GeoModelUtilities)
