#
# Utility snippet to set a default build type if none was specified by the user
# at command line.
# 
# Source: https://blog.kitware.com/cmake-and-the-default-build-type/ by M.D.Hanwell
#

# Set a default build type if none was specified
set(default_build_type "Release")
# If the source folder seems to contain a .git folder, 
# then we assume the user is developing the code and
# we set a more useful type 
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "RelWithDebInfo")
endif()

# Of course, if the use specified a build type at configuration time
# by using 'CMAKE_BUILD_TYPE', then the one specified by the user will be used.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "-----")
message(STATUS "Building with type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Using C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "-----")
