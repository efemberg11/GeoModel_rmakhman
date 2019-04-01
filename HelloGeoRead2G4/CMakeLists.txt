################################################################################
# Package: HelloGeo
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - Nov, 2018
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(HelloGeoRead2G4)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Dependencies.
## GeoModel dependencies.
find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )
find_package( GeoModelG4 REQUIRED )
## External dependencies.
find_package( Geant4 REQUIRED )

# TODO: remove these dependencies by fixing the corresponding CMake setups
# find_package( TFPersistification REQUIRED ) # Remove this dependency: it is brough in by some other package --> fix the CMake setup of that package
# find_package( CLHEP ) # Remove this dependency: it is brough in by TFPersistification

### External Dependencies
# Find the Qt5 libraries, used e.g. to interface with the SQlite DB
find_package(Qt5 COMPONENTS Core Sql Widgets REQUIRED) # TODO: remove Qt dependency, which comes from GeoModelDBManager
# Instruct CMake to run Qt5 moc automatically when needed
set(CMAKE_AUTOMOC ON) # TODO: do we need this here??

# include Geant4 headers
include(${Geant4_USE_FILE})

# Populate a CMake variable with the sources
set(SRCS main.cpp )

# Tell CMake to create the helloworld executable
add_executable( hellogeoRead2G4 ${SRCS} )

# Link all needed libraries
target_link_libraries( hellogeoRead2G4 Qt5::Core Qt5::Sql GeoModelDBManager GeoModelRead GeoModelKernel GeoModelUtilities GeoModel2G4  ${Geant4_LIBRARIES})

# Set include directories
target_include_directories( hellogeoRead2G4 SYSTEM PUBLIC ${GeoModelRead_INCLUDE_DIRS} ${GeoModelUtilities_INCLUDE_DIRS} ${GeoModelG4_INCLUDE_DIRS} ${Geant4_INCLUDE_DIRS})
