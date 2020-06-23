################################################################################
# Package: GeoTessellatedSolidExample
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Sep, 2019
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoTessellatedSolidExample)

# Print Build Info on screen
include(../cmake/PrintBuildInfo.cmake)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore 3.2.0 REQUIRED )
find_package( GeoModelIO 3.2.0 REQUIRED )
find_package( Eigen3 REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager


# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( geoTessellatedExample ${SRCS} )

# Link all needed libraries
target_link_libraries( geoTessellatedExample GeoModelCore::GeoModelKernel GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelWrite Qt5::Core )

# Set include directories
#target_include_directories( geoTessellatedExample SYSTEM PUBLIC ${GeoModelWrite_INCLUDE_DIRS} ${GeoModelKernel_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} )
