
# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoSpecialShapes
################################################################################

find_package(CLHEP)
find_package(GeoModelCore)

# Use the GNU install directory names.
include( GNUInstallDirs )  # it defines CMAKE_INSTALL_LIBDIR

# Find the header and source files.
file( GLOB SOURCES src/*.cxx src/LArWheelCalculator_Impl/*.cxx )
file( GLOB HEADERS GeoSpecialShapes/*.h src/LArWheelCalculator_Impl/*.h)

# Set the library.
add_library( GeoSpecialShapes SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoSpecialShapes PUBLIC
PRIVATE GeoModelCore::GeoModelKernel ${CLHEP_LIBRARIES} )
target_include_directories( GeoSpecialShapes PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoSpecialShapes" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoSpecialShapes PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


# new test GeoModelG4
install( TARGETS GeoSpecialShapes EXPORT GeoSpecialShapes-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoSpecialShapes )



