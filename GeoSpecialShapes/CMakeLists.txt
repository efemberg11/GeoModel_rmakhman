################################################################################
# Package: GeoSpecialShapes
################################################################################

cmake_minimum_required(VERSION 3.10)

# Declare the package name
project( "GeoSpecialShapes" VERSION 1.0.0 LANGUAGES CXX )

# Declare the package's dependencies:
#atlas_depends_on_subdirs( PUBLIC
#                          Control/AthenaKernel
#                          Control/StoreGate
#                          PRIVATE
#                          Control/CxxUtils
#                          Database/RDBAccessSvc
#                          DetectorDescription/GeoModel/GeoModelInterfaces
#                          DetectorDescription/GeoModel/GeoModelUtilities
#                          GaudiKernel )

# External dependencies:
find_package ( GeoModelCore REQUIRED)

# Project's Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx src/LArWheelCalculator_Impl/*.cxx )
file( GLOB HEADERS GeoSpecialShapes/*.h src/LArWheelCalculator_Impl/*.h)

# Set target and properties
add_library( GeoSpecialShapes SHARED ${HEADERS} ${SOURCES} )

target_link_libraries( GeoSpecialShapes PUBLIC
PRIVATE GeoMaterial2G4 GeoModel2G4
GeoModelCore::GeoModelKernel )

#target_include_directories( GeoSpecialShapes PUBLIC  ${GeoModelCore_INCLUDE_DIRS}
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#   $<INSTALL_INTERFACE:include> )

target_include_directories( GeoSpecialShapes SYSTEM PUBLIC  PRIVATE ${GeoModelCore_INCLUDE_DIRS} )
target_include_directories( GeoSpecialShapes PUBLIC ${GeoModelCore_INCLUDE_DIRS}
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )


# Set installation of library headers
set_property( TARGET GeoSpecialShapes PROPERTY PUBLIC_HEADER ${HEADERS} )

# new test GeoSpecialShapes
install( TARGETS GeoSpecialShapes EXPORT GeoSpecialShapes-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoSpecialShapes )



