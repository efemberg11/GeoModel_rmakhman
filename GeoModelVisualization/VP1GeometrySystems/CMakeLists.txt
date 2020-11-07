################################################################################
# Package: VP1GeometrySystems
################################################################################


set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)
project ( "GXGeometrySystems" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )
find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
# find_package( Eigen3 REQUIRED )
find_package(nlohmann_json QUIET)
if ( APPLE )
  find_package(OpenGL REQUIRED)
endif()

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometrySystems/*.h )
file( GLOB UIS src/*.ui VP1GeometrySystems/*.ui ) # Qt5 UI files
file( GLOB DATA share/*.jpg share/*.json )


set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt5 settings
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")

add_library ( GXGeometrySystems SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )

# If we use standalone-build of the nlohmann_json library, add explicit dependency
if( GEOMODEL_USE_BUILTIN_JSON )
  add_dependencies( GXGeometrySystems JSONExt )
  # Acquire Installation Directory of JSONExt
  ExternalProject_Get_Property (JSONExt install_dir)
  # Include the installed 'include' PATH
  include_directories (${install_dir}/include)
endif()


# We link to `nlohmann_json` only  if we use a version of nlohmann_json
# that provides a CMake config file (i.e., either built from source, or also
# installed with Homebrew on macOS).
# This is not needed if the single-header library is installed in a regular
# system include folder (e.g., '/usr/local/include', '/usr/include', ...)
if ( nlohmann_json_FOUND )
    target_link_libraries( GXGeometrySystems PUBLIC nlohmann_json::nlohmann_json )
endif()
# Check if we are building GeoModelVisualization individually,
# or as a part of the main GeoModel.
# In the first case, we link against the imported targets, which are taken
# from the base packages already installed on the system. 
# This is used when building distribution packages. 
if (${VISUALIZATION_INDIVIDUAL_BUILD}) # if built individually
    target_link_libraries( GXGeometrySystems PUBLIC GeoModelCore::GeoModelKernel )
    target_link_libraries( GXGeometrySystems PUBLIC GeoModelIO::GeoModelRead GeoModelIO::GeoModelWrite )
else() # if built as a part of GeoModel
    target_link_libraries( GXGeometrySystems PUBLIC GeoModelKernel )
    target_link_libraries( GXGeometrySystems PUBLIC GeoModelRead GeoModelWrite )
endif()
target_link_libraries (GXGeometrySystems PUBLIC SoQt::SoQt Coin::Coin Qt5::Core 
    GXGui GXBase GXHEPVis Qt5::Gui Qt5::Widgets Qt5::PrintSupport )
if ( APPLE )
  target_link_directories (GXGeometrySystems PUBLIC ${Coin_LIB_DIR} )
endif()

# Set a variable storing the CMake install prefix.
# -- It is passed to the C++ preprocessor to load, from the C++ code,
#    the JSON files storing the default materials and volume attributes
#    for Coin3D.
add_definitions ( -DGEOMODEL_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})

install(TARGETS GXGeometrySystems
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )
install( FILES ${DATA} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/gmex)

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXGeometrySystems PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
