################################################################################
# Package: HelloGeo2G4
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloGeoRead2G4)

# Compile with C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Print Build Info on screen
include(../cmake/PrintBuildInfo.cmake)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore 3.2.0 REQUIRED )
find_package( GeoModelIO 3.2.0 REQUIRED )
find_package( GeoModelG4 REQUIRED )
## External dependencies.
find_package( Geant4 REQUIRED )

### External Dependencies
# Find the Qt5 libraries, used e.g. to interface with the SQlite DB
find_package(Qt5 COMPONENTS Core Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager
# Instruct CMake to run Qt5 moc automatically when needed
set(CMAKE_AUTOMOC ON) # TODO: do we need this here??

# include Geant4 headers
include(${Geant4_USE_FILE})

# Populate a CMake variable with the sources
set( SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( hellogeoRead2G4 ${SRCS} )

# Link all needed libraries
target_link_libraries( hellogeoRead2G4 Qt5::Core GeoModelIO::GeoModelDBManager GeoModelIO::GeoModelRead GeoModelCore::GeoModelKernel GeoModel2G4  ${Geant4_LIBRARIES})
