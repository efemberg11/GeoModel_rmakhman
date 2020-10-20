#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(fullSimLight)

# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(MagneticField)


# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#include(${ROOT_USE_FILE})


## External dependencies.
#----------------------------------------------------------------------------
# Find Geant4 package, batch mode only executable (i.e. no need ui and vis).
#
find_package(Geant4 REQUIRED)
#----------------------------------------------------------------------------

find_package(Pythia)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
set(OUTPUT bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT})

add_executable(fullSimLight fullSimLight.cc ${sources} ${headers})
add_executable(gmclash geoModelClash.cc ${sources} ${headers})
add_executable(fillHistogramExample fillHistogramExample.cc src/Histo.cc include/Histo.hh)
add_executable(gmgeantino geantinoMaps.cc ${sources} ${headers})
#add_executable(plotGeantinoMaps plotGeantinoMaps.cc)
add_executable(testMagneticField testMagneticField.cc)
add_executable(gmtogdml geoModeltoGDML.cc ${sources} ${headers})

if(Pythia_FOUND)
  target_compile_definitions(fullSimLight PRIVATE USE_PYTHIA)
  target_link_libraries(fullSimLight Pythia::Pythia)
endif()

#----------------------------------------------------------------------------
# Add extra 'include' directories
#
# If the in-house build of the nlohmann_json library is used, add explicit dependency
if( GEOMODEL_USE_BUILTIN_JSON )
  add_dependencies( fullSimLight JSONExt )
  add_dependencies( gmclash JSONExt )
  add_dependencies( fillHistogramExample JSONExt )
  add_dependencies( gmgeantino JSONExt )
  # add_dependencies( plotGeantinoMaps JSONExt )
  add_dependencies( testMagneticField JSONExt )
  add_dependencies( gmtogdml JSONExt )
  # Acquire Installation Directory of JSONExt
  ExternalProject_Get_Property (JSONExt install_dir)
  # Include the installed 'include' PATH
  include_directories (${install_dir}/include)
endif()

#----------------------------------------------------------------------------
# Link all needed libraries
#
target_link_libraries(fillHistogramExample ${Geant4_LIBRARIES})
target_link_libraries(testMagneticField ${Geant4_LIBRARIES} MagFieldServices MagFieldInterfaces)
#target_link_libraries(plotGeantinoMaps ${ROOT_LIBRARIES})

# targets that need 'nlohmann_json'
# NOTE: We link to `nlohmann_json` only  if we use a version of nlohmann_json
# that provides a CMake config file (i.e., either built from source, or also
# installed with Homebrew on macOS).
# This is not needed if the single-header library is installed in a regular
# system include folder (e.g., '/usr/local/include', '/usr/include', ...)
target_link_libraries(gmclash GeoModelDBManager GeoModelRead GeoModelKernel GeoModel2G4 ${Geant4_LIBRARIES} MagFieldServices MagFieldInterfaces)
target_link_libraries(fullSimLight GeoModelDBManager GeoModelRead GeoModelKernel GeoModel2G4 ${Geant4_LIBRARIES} MagFieldServices MagFieldInterfaces)
target_link_libraries(gmgeantino GeoModelDBManager GeoModelRead GeoModelKernel GeoModel2G4 ${Geant4_LIBRARIES} MagFieldServices MagFieldInterfaces)
target_link_libraries(gmtogdml GeoModelDBManager GeoModelRead GeoModelKernel GeoModel2G4 ${Geant4_LIBRARIES} MagFieldServices MagFieldInterfaces)
if( nlohmann_json_FOUND )
  target_link_libraries( gmclash nlohmann_json::nlohmann_json )
  target_link_libraries( fullSimLight nlohmann_json::nlohmann_json )
  target_link_libraries( gmgeantino nlohmann_json::nlohmann_json )
  target_link_libraries( gmtogdml nlohmann_json::nlohmann_json )
else()
  message(STATUS "WARNING - 'nlohmann_json' not found by CMake!! Anyway, if you installed the single header file in a standard system include dir, I will be able to use it.")
endif()

#ROOT_GENERATE_DICTIONARY(G__geantinoMaps ${headers} LINKDEF geantinoMapsLinkDef.h)

#----------------------------------------------------------------------------
# Copy all scripts to the build/OUTPUT directory. This is so that, after
# install, we can run the executable directly because it relies on these
# scripts being in the current working directory.
#
set(FULLSIMLIGHT_SCRIPTS
  geantino.g4
  macro.g4
  drawMagField.C
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
install(TARGETS fillHistogramExample DESTINATION ${OUTPUT})
install(TARGETS gmgeantino DESTINATION ${OUTPUT})
install(TARGETS testMagneticField DESTINATION ${OUTPUT})
install(TARGETS gmtogdml DESTINATION ${OUTPUT})
install(FILES ${FULLSIMLIGHT_SCRIPTS} DESTINATION share/FullSimLight)
