
#
# Set build options and C++ standards and options
#
# This file sets up
#
#   CMAKE_BUILD_TYPE
#   CMAKE_CXX_STANDARD
#   CMAKE_CXX_EXTENSIONS
#   CMAKE_CXX_STANDARD_REQUIRED
#
# The options can be overridden at configuration time by using, e.g.:
#    `cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_STANDARD=14 ../GeoModelIO`
# on the command line.
#


# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "Require the specified C++ standard for the build" )
