set (CMAKE_CXX_FLAGS "-g -O0 -gdwarf-2")

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GUI/*.h )
file( GLOB UIS src/*.ui VP1GUI/*.ui )


# find_package( SoQt )
# find_package( Coin )
# find_package(OpenGL REQUIRED)


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

set (UI_RESOURCES src/vp1.qrc)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/VP1GUI/src)
QT5_WRAP_UI(UI_HDRS ${UIS})
QT5_ADD_RESOURCES(UI_RESOURCES_RCC ${UI_RESOURCES})


# Configure package build.
add_library ( VP1GUI SHARED ${SOURCES} ${HEADERS} ${UI_HDRS} ${UI_RESOURCES_RCC}  )
target_link_libraries (VP1GUI VP1Base VP1HEPVis Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport )

# Configure debug output.
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
   target_compile_definitions( VP1GUI PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()


# Configure package installation.
target_include_directories( VP1GUI PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "VP1GUI" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( VP1GUI PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS VP1GUI
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS VP1GUI
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VP1GUI
   COMPONENT Development )
