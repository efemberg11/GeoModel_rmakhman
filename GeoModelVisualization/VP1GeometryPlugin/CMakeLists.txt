################################################################################
# Package: GXGeometryPlugin
################################################################################

set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "GXGeometryPlugin" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin3D REQUIRED )
find_package( SoQt REQUIRED )
find_package( Qt5 COMPONENTS Core Gui Widgets Sql PrintSupport )
if( APPLE )
  find_package(OpenGL REQUIRED)
endif()

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometryPlugin/*.h )
file( GLOB UIS src/*.ui VP1GeometryPlugin/*.ui ) # Qt5 UI files

set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt5 settings
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


# TODO: these include directories should not be explicitly declared, 
#       they should be taken by CMake automatically...
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Base")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1GeometrySystems")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1GuideLineSystems")
include_directories ("${PROJECT_SOURCE_DIR}/../GXClashPointSystems")


add_library ( GXGeometryPlugin SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )

# Check if we are building GeoModelVisualization individually,
# or as a part of the main GeoModel.
# In the first case, we link against the imported targets, which are taken
# from the base packages already installed on the system.
# This is used when building distribution packages.
if (${VISUALIZATION_INDIVIDUAL_BUILD}) # if built individually
    target_link_libraries( GXGeometryPlugin PUBLIC GeoModelCore::GeoModelKernel )
    target_link_libraries( GXGeometryPlugin PUBLIC GeoModelIO::GeoModelRead GeoModelIO::GeoModelDBManager GeoModelIO::TFPersistification GeoModelCore::GeoGenericFunctions GeoModelCore::GeoModelKernel )
else() # if built as a part of GeoModel
    target_link_libraries( GXGeometryPlugin PUBLIC GeoModelKernel )
    target_link_libraries( GXGeometryPlugin PUBLIC GeoModelRead GeoModelDBManager TFPersistification GeoGenericFunctions GeoModelKernel )
endif()
target_link_libraries (GXGeometryPlugin PUBLIC GXGeometrySystems GXGuideLineSystems  GXClashPointSystems GXGui GXBase GXHEPVis SoQt::SoQt ${COIN3D_LIBRARIES} Qt5::Gui Qt5::Core Qt5::Widgets Qt5::PrintSupport )

install(TARGETS GXGeometryPlugin
  LIBRARY
  DESTINATION lib/gxplugins
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXGeometryPlugin PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})
