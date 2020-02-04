################################################################################
# Package: VP1GeometryPlugin
################################################################################

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS VP1GeometryPlugin/*.h )
file( GLOB UIS src/*.ui VP1GeometryPlugin/*.ui )


set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
unset (AUTOUIC_SEARCH_PATHS)
set (CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/src)
QT5_WRAP_UI(UI_HDRS ${UIS})


# Configure package build.
add_library ( GXGeometryPlugin SHARED ${SOURCES} ${HEADERS} ${UI_HDRS}  )
target_link_libraries (GXGeometryPlugin VP1GeometrySystems VP1GUI VP1Base VP1HEPVis )

# Configure debug output
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
"${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
target_compile_definitions( GXGeometryPlugin PUBLIC
-DQT_NO_DEBUG_OUTPUT )
endif()

# Configure package installation.
target_include_directories( GXGeometryPlugin PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "VP1GeometryPlugin" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GXGeometryPlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the actual library (i.e., libGXGeometryPlugin.1.dylib)
install( TARGETS GXGeometryPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gxplugins
   COMPONENT Runtime
   NAMELINK_SKIP )
# Install the symlink (i.e., libGXGeometryPlugin.dylib)
install( TARGETS GXGeometryPlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gxplugins
   COMPONENT Development
   NAMELINK_ONLY )
# Install the header files
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GXGeometryPlugin
   COMPONENT Development )
