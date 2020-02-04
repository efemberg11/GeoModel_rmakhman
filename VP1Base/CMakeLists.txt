
find_package( Coin )
find_package( SoQt  )
if ( APPLE )
	find_package(OpenGL REQUIRED)
endif()
find_package( Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport )



# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1Base/*.h )
file( GLOB UIS src/*.ui )


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)
set (AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src )
set (UI_RESOURCES src/icons.qrc)
QT5_WRAP_UI(UI_HDRS ${UIS})
QT5_ADD_RESOURCES(UI_RESOURCES_RCC ${UI_RESOURCES})


# Configure package build.
add_library ( VP1Base SHARED ${SOURCES} ${HEADERS} ${UI_HDRS} ${UI_RESOURCES_RCC} )
if (APPLE)
  target_link_libraries( VP1Base PUBLIC VP1HEPVis Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::PrintSupport Qt5::Gui Coin SoQt )
else()
  target_link_libraries( VP1Base PUBLIC VP1HEPVis Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::PrintSupport Qt5::Gui )
endif()

# Configure debug output
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
"${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
target_compile_definitions( VP1Base PUBLIC
-DQT_NO_DEBUG_OUTPUT )
endif()


# Configure package installation.
target_include_directories( VP1Base PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "VP1Base" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1Base PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS VP1Base
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS VP1Base
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1Base
   COMPONENT Development )
