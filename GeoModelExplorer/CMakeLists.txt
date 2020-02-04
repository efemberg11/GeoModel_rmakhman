set(CMAKE_CXX_FLAGS "-fPIC -O0 -g -gdwarf-2" )


find_package( SoQt  )
if ( APPLE )
  find_package( Coin )
else()
  find_package( Coin3D )
endif()


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

# Set rpath on macOS
if( APPLE )
   set(CMAKE_MACOSX_RPATH 1)
endif()

# Configure package build.
add_executable ( gmex ${SOURCES} ${HEADERS}  )
target_link_libraries( gmex VP1GUI VP1Base VP1HEPVis SoQt Coin Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport )

# Configure debug output
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
"${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
target_compile_definitions( gmex PUBLIC
-DQT_NO_DEBUG_OUTPUT )
endif()

# Configure package installation.
install(TARGETS gmex DESTINATION bin)
