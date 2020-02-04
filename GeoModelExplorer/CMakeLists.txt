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
# Docs: https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling#always-full-rpath
if( APPLE )
  # With these settings you are able to run the programs directly from the build tree.
  # But the RPATH is cleared when installing the executable.
  # So we have to configure the RPATH handling farther, after this instruction.
  SET(CMAKE_MACOSX_RPATH 1)

  # use, i.e. don't skip the full RPATH for the build tree
  SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

  # when building, don't use the install RPATH already
  # (but later on when installing)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )

  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


  # the RPATH to be used when installing, but only if it's not a system directory
  LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  IF("${isSystemDir}" STREQUAL "-1")
     SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  ENDIF("${isSystemDir}" STREQUAL "-1")
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
