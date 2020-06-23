################################################################################
# Package: Geo2G4
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "Geo2G4" VERSION 1.0.0 LANGUAGES CXX )

# External dependencies:
find_package( Geant4 REQUIRED )
find_package( Boost COMPONENTS filesystem thread system )
find_package( CLHEP )
find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist pthread )
find_package( XercesC )
# GeoModel dependencies
find_package( GeoModelKernel REQUIRED )
find_package( GeoModelUtilities REQUIRED )
find_package( GeoMaterial2G4 REQUIRED )

##########################################################
# NOTE! The original package also needs this Athena stuff:
#
# DetectorDescription/GeoModel/GeoModelInterfaces
# DetectorDescription/GeoModel/GeoSpecialShapes
# DetectorDescription/GeoPrimitives
# Simulation/G4Atlas/G4AtlasInterfaces
# Simulation/G4Atlas/G4AtlasTools
# Simulation/G4Sim/SimHelpers

# Project's Settings

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoMaterial2G4/*.h )

# include Geant4 headers
include(${Geant4_USE_FILE})

# Set target and properties
add_library( Geo2G4Lib SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( Geo2G4Lib
  PUBLIC ${XERCESC_LIBRARIES} ${GEANT4_LIBRARIES} ${ROOT_LIBRARIES}
  PRIVATE ${Boost_LIBRARIES} ${CLHEP_LIBRARIES} ${GEOMODEL_LIBRARIES} GeoSpecialShapes G4AtlasToolsLib SimHelpers GeoMaterial2G4  GeoModelUtilities )
target_include_directories( Geo2G4Lib SYSTEM PUBLIC ${XERCESC_INCLUDE_DIRS} ${GEANT4_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} PRIVATE ${Boost_INCLUDE_DIRS} ${CLHEP_INCLUDE_DIRS} ${GEOMODEL_INCLUDE_DIRS} )
target_include_directories( Geo2G4Lib PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

### TODO: do we need this from Athena? Maybe not...
# atlas_add_component( Geo2G4 ... )

### TODO: do we need this from Athena? Maybe not...
# atlas_add_dictionary( LArWheelSolidCheckerDict ... )


