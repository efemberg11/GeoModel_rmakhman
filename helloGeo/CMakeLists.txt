################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloGeo)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
## External Dependencies.
find_package(Qt5 COMPONENTS Core Sql Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager

# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( hellogeo ${SRCS} )

# Link all needed libraries
target_link_libraries( hellogeo Qt5::Core Qt5::Sql GeoModelDBManager GeoModelRead GeoModelKernel GeoModelUtilities)

# Set include directories
target_include_directories( hellogeo SYSTEM PUBLIC ${GeoModelRead_INCLUDE_DIRS} ${GeoModelUtilities_INCLUDE_DIRS} )
