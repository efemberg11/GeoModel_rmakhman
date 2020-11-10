
# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoSpecialShapes
################################################################################

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# include Geant4 headers
include(${Geant4_USE_FILE})

# Find the header and source files.
file( GLOB SOURCES src/*.cxx src/LArWheelCalculator_Impl/*.cxx )
file( GLOB HEADERS GeoSpecialShapes/*.h src/LArWheelCalculator_Impl/*.h)

# Set the library.
add_library( GeoSpecialShapes SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoSpecialShapes PRIVATE ${Geant4_LIBRARIES})
# Check if we are building FullSimLight individually,
# or as a part of the main GeoModel.
# In the first case, we link against the imported targets, which are taken
# from the base GeoModel packages already installed on the system.^[OB
# This is used when building distribution packages.
if ( GeoModelG4_INDIVIDUAL_BUILD ) # if built individually
    target_link_libraries( GeoSpecialShapes PRIVATE GeoModelCore::GeoModelKernel )
else()
    target_link_libraries( GeoSpecialShapes PRIVATE GeoModelKernel )
endif()

target_include_directories( GeoSpecialShapes PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> ${GEANT4_INCLUDE_DIRS})
source_group( "GeoSpecialShapes" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoSpecialShapes PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


# new test GeoModelG4
install( TARGETS GeoSpecialShapes EXPORT GeoSpecialShapes-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoSpecialShapes )
