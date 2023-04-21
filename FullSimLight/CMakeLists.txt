# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# CMake settings
cmake_minimum_required(VERSION 3.16...3.26)

# Dummy call to 'project()', needed to set 'PROJECT_SOURCE_DIR'
project( "FullSimLight" )

#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )
    # get global GeoModel version
    include( GeoModel-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "FullSimLight" VERSION ${GeoModel_VERSION} LANGUAGES CXX )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set a default build type
    include( BuildType )
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Set up how the project handle some of its dependenices. Either by picking them
    # up from the environment, or building them itself.
    if( NOT TARGET JSONExt )
        include( SetupJSON )
    endif()
    if( NOT TARGET XercesCBuiltIn )
        include( SetupXercesC )
    endif()
    # Find the base GeoModel packages, which must be installed on the target system already
    find_package( GeoModelCore REQUIRED ${GeoModel_VERSION} )
    find_package( GeoModelIO REQUIRED ${GeoModel_VERSION} )
    find_package( GeoModelG4 REQUIRED ${GeoModel_VERSION} )
    # Set a flag to steer the  of the subpackages
    set( ${PROJECT_NAME}_INDIVIDUAL_BUILD ON )
    # Print Build Info on screen
    include( PrintBuildInfo )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModel project.")
    # Set the project
    project( "FullSimLight" VERSION ${GeoModel_VERSION} LANGUAGES CXX )
endif()


## External dependencies.
#----------------------------------------------------------------------------
# Find Geant4 package, batch mode only executable (i.e. no need ui and vis).
#
find_package(Geant4 REQUIRED)
message( STATUS "Found Geant4: ${Geant4_INCLUDE_DIR}")
#----------------------------------------------------------------------------


# Support for the HepMC3 exchange format
# This is OFF by default, but it can be enbled by the user if used/needed;
# for that, of course, HepMC3 must be installed on the system.
option(GEOMODEL_USE_HEPMC3 "Build GeoModel tools with support for the HepMC3 exchancge format (Note: HepMC3 must be installed on the target machine)" OFF )
if(GEOMODEL_USE_HEPMC3)
    find_package(HepMC3 REQUIRED) # if support for HEPMC3 is enabled, it must be installed.
    if(HepMC3_FOUND)
        message( STATUS "Found HepMC3 (${HEPMC3_INCLUDE_DIR}), and support for HepMC3 is enabled; so, support for it will be compiled.")
        add_compile_definitions( USE_HEPMC3 )
    endif()
else()
    message(STATUS "! NOTE ==> The support for HepMC3 is disabled by default, and it is not compiled at the moment. However, it can be enabled by the user by giving the option '-DGEOMODEL_USE_HEPMC3=1' to CMake; in that case, you must be sure HepMC3 is installed on the system.")
endif()
# If HepMC3 is not found, weather it is enabled or not by the user, 
# we remove from the build the sources that use it
if( NOT HepMC3_FOUND)
    file(GLOB hepmc3src ${PROJECT_SOURCE_DIR}/src/HepMC3*.cc)
    #message("Removing those HepMC3-related sources: ${hepmc3src}") # debug msg
    list(REMOVE_ITEM sources ${hepmc3src})
    #message("new sources without HepMC3 support: ${sources}") # debug msg
endif()

# Support for Pythia event generator
# This is OFF by default, but it can be enbled by the user if used/needed;
# for that, of course, Pythia must be installed on the system.

option(GEOMODEL_USE_PYTHIA "Build GeoModel tools with support for the Pythia event generator  (Note: Pythia must be installed on the target machine)" OFF )

if(GEOMODEL_USE_PYTHIA)
    find_package(Pythia REQUIRED) # if support for Pythia is enabled, it must be installed.
    if(Pythia_FOUND)
      message( STATUS "Found Pythia (${Pythia_INCLUDE_DIR}), and support for Pythia is enabled; so, support for it will be compiled.")
      add_compile_definitions( USE_PYTHIA )
    endif()
else()
    message(STATUS "! NOTE ==> The support for Pythia is disabled by default, and it is not compiled at the moment. However, it can be enabled by the user by giving the option '-DGEOMODEL_USE_Pythia=1' to CMake; in that case, you must be sure Pythia is installed on the system.")
endif()






#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
#message("Geant4_USE_FILE: ${Geant4_USE_FILE}") # debug msg
include(${Geant4_USE_FILE})
add_definitions (-DG4VERSION="${Geant4_VERSION}" )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Add sub-projects and targets
add_subdirectory(Plugins)


# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#include(${ROOT_USE_FILE})



include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)
#message("sources: ${sources}") # debug msg


# Remove from the build HepMC3-related sources if that is not found
# TODO: remove these lines and put pre-comp fences with 'USE_HEPMC3' def
if( NOT HepMC3_FOUND)
    file(GLOB hepmc3src ${PROJECT_SOURCE_DIR}/src/HepMC3*.cc)
    #message("Removing those HepMC3-related sources: ${hepmc3src}") # debug msg
    list(REMOVE_ITEM sources ${hepmc3src})
    #message("new sources without HepMC3 support: ${sources}") # debug msg
endif()


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
set(OUTPUT bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT})

add_definitions (-DG4SHAREDIR="${Geant4_INCLUDE_DIR}/../../share" )
add_definitions (-DFULLSIMLIGHTSHAREDIR="${CMAKE_INSTALL_PREFIX}/share/FullSimLight" )

add_executable(fullSimLight fullSimLight.cc ${sources} ${headers})
add_executable(gmclash geoModelClash.cc ${sources} ${headers})
add_executable(gmmasscalc geoModelMassCalculator.cc ${sources} ${headers})
add_executable(fillHistogramExample fillHistogramExample.cc src/Histo.cc include/Histo.hh)
add_executable(gmgeantino geantinoMaps.cc ${sources} ${headers})
#add_executable(plotGeantinoMaps plotGeantinoMaps.cc)
#add_executable(testMagneticField testMagneticField.cc)
add_executable(gm2gdml geoModel2GDML.cc ${sources} ${headers})




#if(HEPMC3_ROOTIO_USE)
#  message( STATUS "HEPMC3 Root Dependency Switched on")
#  target_link_libraries(gmclash PUBLIC  ${HEPMC3_ROOTIO_LIB} ${ROOT_LIBRARIES})
 # target_link_libraries(gmmasscalc PUBLIC  ${HEPMC3_ROOTIO_LIB} ${ROOT_LIBRARIES})
 # target_link_libraries(fullSimLight PUBLIC ${HEPMC3_ROOTIO_LIB} ${ROOT_LIBRARIES})
 # target_link_libraries(gmgeantino PUBLIC  ${HEPMC3_ROOTIO_LIB} ${ROOT_LIBRARIES})
 # target_link_libraries(gm2gdml PUBLIC ${HEPMC3_ROOTIO_LIB} ${ROOT_LIBRARIES})
 # target_include_directories(gmclash PUBLIC ${ROOT_INCLUDE_DIRS})
 # target_include_directories(gmmasscalc PUBLIC  ${ROOT_INCLUDE_DIRS})
 # target_include_directories(fullSimLight PUBLIC  ${ROOT_INCLUDE_DIRS})
 # target_include_directories(gmgeantino PUBLIC  ${ROOT_INCLUDE_DIRS})
 # target_include_directories(gm2gdml PUBLIC  ${ROOT_INCLUDE_DIRS})

 # target_compile_definitions(gmclash PRIVATE USE_ROOT)
  #target_compile_definitions(gmmasscalc PRIVATE USE_ROOT)
 # target_compile_definitions(fullSimLight PRIVATE USE_ROOT)
 # target_compile_definitions(gmgeantino PRIVATE USE_ROOT)
 # target_compile_definitions(gm2gdml PRIVATE USE_ROOT)
#endif()

#----------------------------------------------------------------------------
# Add extra 'include' directories
#
# If the in-house build of the nlohmann_json library is used, add explicit dependency
if( GEOMODEL_USE_BUILTIN_JSON )
  add_dependencies( fullSimLight JSONExt )
  add_dependencies( gmclash JSONExt )
  add_dependencies( gmmasscalc JSONExt )
  add_dependencies( fillHistogramExample JSONExt )
  add_dependencies( gmgeantino JSONExt )
  #add_dependencies( testMagneticField JSONExt )
  add_dependencies( gm2gdml JSONExt )
  # Acquire Installation Directory of JSONExt
  ExternalProject_Get_Property (JSONExt install_dir)
  # Include the installed 'include' PATH
  include_directories (${install_dir}/include)
endif()
# If the in-house build of the Xerces-C library is used, add explicit dependency: for Geant4
if( GEOMODEL_USE_BUILTIN_XERCESC )
    add_dependencies( fullSimLight XercesCBuiltIn )
    add_dependencies( gmclash XercesCBuiltIn )
    add_dependencies( gmmasscalc XercesCBuiltIn )
    add_dependencies( fillHistogramExample XercesCBuiltIn )
    add_dependencies( gmgeantino XercesCBuiltIn )
#    add_dependencies( testMagneticField XercesCBuiltIn )
    add_dependencies( gm2gdml XercesCBuiltIn )
endif()
#----------------------------------------------------------------------------
# Link all needed libraries
#
target_link_libraries(fillHistogramExample ${Geant4_LIBRARIES})
#target_link_libraries(testMagneticField ${Geant4_LIBRARIES}  MagFieldInterfaces)
#target_link_libraries(plotGeantinoMaps ${ROOT_LIBRARIES})


target_link_libraries(gmclash PUBLIC GeoModel2G4 ${Geant4_LIBRARIES} )
target_link_libraries(gmmasscalc PUBLIC GeoModel2G4 ${Geant4_LIBRARIES} )
target_link_libraries(fullSimLight PUBLIC GeoModel2G4 ${Geant4_LIBRARIES} )
target_link_libraries(gmgeantino PUBLIC GeoModel2G4 ${Geant4_LIBRARIES} )
target_link_libraries(gm2gdml PUBLIC GeoModel2G4 ${Geant4_LIBRARIES} )


if( HepMC3_FOUND)
    target_link_libraries(gmclash PUBLIC GeoModel2G4 ${HEPMC3_LIB})
    target_link_libraries(gmmasscalc PUBLIC GeoModel2G4 ${HEPMC3_LIB})
    target_link_libraries(fullSimLight PUBLIC GeoModel2G4 ${HEPMC3_LIB})
    target_link_libraries(gmgeantino PUBLIC GeoModel2G4 ${HEPMC3_LIB})
    target_link_libraries(gm2gdml PUBLIC GeoModel2G4 ${HEPMC3_LIB})
    target_include_directories(gmclash PUBLIC ${HEPMC3_INCLUDE_DIR})
    target_include_directories(gmmasscalc PUBLIC ${HEPMC3_INCLUDE_DIR})
    target_include_directories(fullSimLight PUBLIC ${HEPMC3_INCLUDE_DIR})
    target_include_directories(gmgeantino PUBLIC ${HEPMC3_INCLUDE_DIR})
    target_include_directories(gm2gdml PUBLIC ${HEPMC3_INCLUDE_DIR})
endif()

if(Pythia_FOUND)
  target_compile_definitions(fullSimLight PRIVATE USE_PYTHIA)
  target_compile_definitions(gmgeantino PRIVATE USE_PYTHIA)
  target_compile_definitions(gmmasscalc PRIVATE USE_PYTHIA)
  target_compile_definitions(gm2gdml PRIVATE USE_PYTHIA)
  target_compile_definitions(gmclash PRIVATE USE_PYTHIA)
  target_link_libraries(fullSimLight PUBLIC Pythia::Pythia)
  target_link_libraries(gmgeantino PUBLIC Pythia::Pythia)
  target_link_libraries(gmmasscalc PUBLIC Pythia::Pythia)
  target_link_libraries(gm2gdml PUBLIC Pythia::Pythia)
  target_link_libraries(gmclash PUBLIC Pythia::Pythia)
  target_include_directories(fullSimLight PUBLIC ${Pythia_INCLUDE_DIR})
  target_include_directories(gmgeantino PUBLIC ${Pythia_INCLUDE_DIR})
  target_include_directories(gmmasscalc PUBLIC ${Pythia_INCLUDE_DIR})
  target_include_directories(gm2gdml PUBLIC ${Pythia_INCLUDE_DIR})
  target_include_directories(gmclash PUBLIC ${Pythia_INCLUDE_DIR})
endif()


# Check if we are building FullSimLight individually,
# or as a part of the main GeoModel.
# In the first case, we link against the imported targets, which are taken
# from the base GeoModel packages already installed on the system.
# This is used when building distribution packages.
if ( ${PROJECT_NAME}_INDIVIDUAL_BUILD ) # if built individually
    target_link_libraries( gmclash PUBLIC GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
    target_link_libraries( gmmasscalc PUBLIC GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
    target_link_libraries( fullSimLight PUBLIC GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
    target_link_libraries( gmgeantino PUBLIC GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
    target_link_libraries( gm2gdml PUBLIC GeoModelCore::GeoModelKernel GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
else() # if built as a part of GeoModel
    target_link_libraries( gmclash PUBLIC GeoModelKernel GeoModelRead GeoModelWrite )
    target_link_libraries( gmmasscalc PUBLIC GeoModelKernel GeoModelRead GeoModelWrite )
    target_link_libraries( fullSimLight PUBLIC GeoModelKernel GeoModelRead GeoModelWrite )
    target_link_libraries( gmgeantino PUBLIC GeoModelKernel GeoModelRead GeoModelWrite )
    target_link_libraries( gm2gdml PUBLIC GeoModelKernel GeoModelRead GeoModelWrite )
endif()

# targets that need 'nlohmann_json'
# NOTE: We link to `nlohmann_json` only  if we use a version of nlohmann_json
# that provides a CMake config file (i.e., either built from source, or also
# installed with Homebrew on macOS).
# This is not needed if the single-header library is installed in a regular
# system include folder (e.g., '/usr/local/include', '/usr/include', ...)
if( nlohmann_json_FOUND )
    target_link_libraries( gmclash PRIVATE nlohmann_json::nlohmann_json )
    target_link_libraries( gmmasscalc PRIVATE nlohmann_json::nlohmann_json )
    target_link_libraries( fullSimLight PRIVATE nlohmann_json::nlohmann_json )
    target_link_libraries( gmgeantino PRIVATE nlohmann_json::nlohmann_json )
    target_link_libraries( gm2gdml PRIVATE nlohmann_json::nlohmann_json )
else()
    message(STATUS "WARNING - 'nlohmann_json' not found by CMake!! Anyway, if you installed the single header file in a standard system include dir, I will be able to use it.")
endif()

#----------------------------------------------------------------------------
# Add profiling test targets
if(GEOMODEL_BUILD_FULLSIMLIGHT_PROFILING)
  if(NOT Pythia_FOUND)
    message(FATAL_ERROR "FullSimLight needs to be built with Pythia support for profiling with perf")
  endif()
  add_subdirectory(perf)
endif()

#ROOT_GENERATE_DICTIONARY(G__geantinoMaps ${headers} LINKDEF geantinoMapsLinkDef.h)

#----------------------------------------------------------------------------
# Copy all scripts to the build/OUTPUT directory. This is so that, after
# install, we can run the executable directly because it relies on these
# scripts being in the current working directory.
#
set(FULLSIMLIGHT_SCRIPTS
  geantino.g4
  geantino_Pixel.g4
  geantino_SCT.g4
  geantino_TRT.g4
  geantino_LAr.g4
  geantino_Tile.g4
  geantino_Muon.g4
  macro.g4
  pythia.g4
  drawMagField.C
  drawGeantinoMaps.C
  )

foreach(_script ${FULLSIMLIGHT_SCRIPTS})
  configure_file(
    ${_script}
    ${CMAKE_BINARY_DIR}/share/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# Install the executable to 'bin/' directory under the
# CMAKE_INSTALL_PREFIX
#


install(TARGETS fullSimLight DESTINATION ${OUTPUT})
install(TARGETS gmclash DESTINATION ${OUTPUT})
install(TARGETS gmmasscalc DESTINATION ${OUTPUT})
install(TARGETS fillHistogramExample DESTINATION ${OUTPUT})
install(TARGETS gmgeantino DESTINATION ${OUTPUT})
#install(TARGETS testMagneticField DESTINATION ${OUTPUT})
install(TARGETS gm2gdml DESTINATION ${OUTPUT})
install(FILES ${FULLSIMLIGHT_SCRIPTS} DESTINATION share/FullSimLight)



add_library ( FullSimLight INTERFACE )

# Set up the packaging of the project using CPack.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include ( GNUInstallDirs )
include( WriteBasicConfigVersionFile )

write_basic_config_version_file(
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion )

install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

#Create and install the description of the libraries.
install( EXPORT ${PROJECT_NAME}-export
   FILE ${PROJECT_NAME}Targets.cmake
   COMPONENT Development
#   NAMESPACE "${PROJECT_NAME}::"
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )

# Install the hand-written project configuration.
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FullSimLightConfig.cmake.in
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   @ONLY )
install(
   FILES
   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake
   COMPONENT Development
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} )


install( TARGETS FullSimLight
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

   
install(FILES FullSimLight/FSLSensitiveDetectorPlugin.h 
              FullSimLight/MagFieldPlugin.h 
              FullSimLight/FSLPhysicsListPlugin.h
	      FullSimLight/FSLUserActionPlugin.h
	          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FullSimLight
  COMPONENT Development )


