################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoShapeExamples)

# Make the module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake )

# Print Build Info on screen
include(PrintBuildInfo)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Sql Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager


# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( geoShapeExamples ${SRCS} )

# Link all needed libraries
target_link_libraries( geoShapeExamples GeoModelCore::GeoModelKernel GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelWrite Qt5::Core Qt5::Sql )
#target_link_libraries( geoShapeExamples GeoModelKernel Qt5::Core Qt5::Sql GeoModelDBManager /Users/rbianchi/code_work_local/atlas/geomodel_packages_tests_21Jun2019/install/lib/libGeoModelWrite.dylib )

# Set include directories
# target_include_directories( geoShapeExamples SYSTEM PUBLIC ${GeoModelWrite_INCLUDE_DIRS} )
