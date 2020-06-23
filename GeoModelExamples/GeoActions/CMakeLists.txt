################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoActions)

# Compile with C++14 //TODO: Should move this to >= C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Print Build Info on screen
include(../cmake/PrintBuildInfo.cmake)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Sql Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager


# Populate a CMake variable with the sources
set(SRCS main.cpp MyVolAction.cxx MyVolActionFilter.cxx )

# Tell CMake to create the helloworld executable
add_executable( geoActions ${SRCS} )

# Link all needed libraries
target_link_libraries( geoActions GeoModelCore::GeoModelKernel Qt5::Core )

# Set include directories
# target_include_directories( geoActions SYSTEM PUBLIC ${GeoModelWrite_INCLUDE_DIRS} )
