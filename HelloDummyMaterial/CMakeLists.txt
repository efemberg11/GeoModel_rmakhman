################################################################################
# Package: HelloDummyMaterial
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2019
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloDummyMaterial)

# Compile with C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
find_package( Eigen3 REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Sql Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager

# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( helloDummyMaterial ${SRCS} )

# Link all needed libraries
target_link_libraries( helloDummyMaterial GeoModelCore::GeoModelKernel Qt5::Core Qt5::Sql GeoModelDBManager GeoModelWrite )

# Set include directories
#target_include_directories( helloDummyMaterial SYSTEM PUBLIC ${GeoModelWrite_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} )
