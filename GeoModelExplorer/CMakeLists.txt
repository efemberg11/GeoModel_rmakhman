set(CMAKE_CXX_FLAGS "-fPIC -O0 -g -gdwarf-2" )


# find_package( SoQt  )
# find_package( Coin )


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )


# Define the path where GMEX plugins are stored and configure the runtime setup script.
set(GMEX_PLUGINPATH "${CMAKE_INSTALL_PREFIX}/lib/gxplugins")
configure_file(scripts/setup_gmex.sh.in setup_gmex.sh)


# Configure the package's build.
add_executable ( gmex ${SOURCES} ${HEADERS}  )
target_link_libraries( gmex VP1Gui VP1Base VP1HEPVis Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport )

# Configure debug output
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
  "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
    target_compile_definitions( gmex PUBLIC
      -DQT_NO_DEBUG_OUTPUT )
endif()

if( NOT APPLE )
  set_target_properties( gmex PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE )
endif()

# Configure the package's installation.
# --- Install the executable.
install( TARGETS gmex RUNTIME DESTINATION bin )
# --- Install the runtime setup script.
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/setup_gmex.sh DESTINATION bin )
