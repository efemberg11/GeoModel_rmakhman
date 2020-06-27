# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Declare the package's executable.
add_executable( gmcat src/gmcat.cxx )
target_link_libraries( gmcat PRIVATE GeoModelKernel
   GeoModelRead GeoModelWrite
   GeoModelDBManager )

# Tweak how debug information should be attached to the executable, in Debug
# builds.
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND
   "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )
   target_compile_options( gmcat PRIVATE "-gdwarf-2" )
endif()

# Install the executable.
install( TARGETS gmcat
   EXPORT ${PROJECT_NAME}-export
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )
