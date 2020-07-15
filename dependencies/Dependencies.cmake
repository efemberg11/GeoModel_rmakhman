# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

message(STATUS "Configuring external dependencies...")

# Make this 'dependencies' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/dependencies )

# Set up how the project handle some of the dependenices.
include( SetupEigen3 )
include( SetupXercesC )
include( SetupJSON )

# Find the project's dependencies
find_package( SQLite3 QUIET )
