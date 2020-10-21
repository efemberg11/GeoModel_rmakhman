# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

message(STATUS "Configuring external dependencies...")

# Make this 'dependencies' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/dependencies )

# By default prefer not to use frameworks on macOS. 
# But use it at a "LAST" resource, if no other options worked.
# For details on the behaviour of this cache variable, see: 
# - https://cmake.org/cmake/help/v3.0/command/find_file.html 
set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
   "Framework finding behaviour on macOS" )


# Set up how the project handle some of the dependenices.
include( SetupEigen3 )
include( SetupXercesC )
include( SetupJSON )

# Find the project's dependencies
find_package( SQLite3 3.7.11 )
