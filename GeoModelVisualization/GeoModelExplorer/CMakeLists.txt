
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "gmex" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport Network )
if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS RunVP1Light/*.h )


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1Gui")
add_executable ( gmex ${SOURCES} ${HEADERS}  )

# Set a variable storing the CMake install prefix.
# -- It is passed to the C++ preprocessor to load the plugins from the C++ code
add_definitions ( -DGEOMODEL_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})

target_link_libraries( gmex GXGui GXBase GXHEPVis ${Qt5Gui_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} )
install(TARGETS gmex DESTINATION bin)
