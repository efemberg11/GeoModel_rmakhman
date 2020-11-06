
set(MYLIB_VERSION_MAJOR 1)
set(MYLIB_VERSION_MINOR 0)
set(MYLIB_VERSION_PATCH 0)

project ( "GXBase" VERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH} LANGUAGES CXX )

find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport )
if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()



# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1Base/*.h )
file( GLOB UIS src/*.ui ) # Qt5 UI files

# Qt5 settings
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)
set (AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src )
set (UI_RESOURCES src/icons.qrc)
QT5_WRAP_UI(UI_HDRS ${UIS})
QT5_ADD_RESOURCES(UI_RESOURCES_RCC ${UI_RESOURCES})


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/../VP1HEPVis")

add_library ( GXBase SHARED ${SOURCES} ${HEADERS} ${UI_HDRS} ${UI_RESOURCES_RCC} )

if ( APPLE )
    target_link_libraries (GXBase ${OPENGL_LIBRARIES} )
endif()
target_link_libraries (GXBase Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::PrintSupport GXHEPVis SoQt::SoQt Coin::Coin )

install(TARGETS GXBase
  LIBRARY
  DESTINATION lib
  COMPONENT Libraries
  )

set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})

set_target_properties(GXBase PROPERTIES VERSION ${MYLIB_VERSION_STRING} SOVERSION ${MYLIB_VERSION_MAJOR})

target_compile_definitions( GXBase PUBLIC -DQT_NO_DEBUG_OUTPUT )
target_compile_definitions( GXBase PUBLIC -DQT_NO_INFO_OUTPUT )
