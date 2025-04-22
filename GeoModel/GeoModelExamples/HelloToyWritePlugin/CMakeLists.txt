# Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: HelloToy
# author: Riccardo Maria BIANCHI @ CERN - Aug, 2020
################################################################################

cmake_minimum_required(VERSION 3.16...3.26)

project(HelloToyWritePlugin)

# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Find the needed dependencies, when building individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) 
  find_package( GeoModelCore REQUIRED  ) 
  find_package( GeoModelIO REQUIRED  ) 
endif()

# Find includes in current dir
file( GLOB SOURCES src/*.cxx )
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
#set(SRCS main.cpp )

add_library( HelloToyWritePlugin SHARED ${SOURCES} )
target_link_libraries( HelloToyWritePlugin PUBLIC GeoModelCore::GeoModelKernel)

source_group( "src" FILES ${SOURCES} )

set_target_properties( HelloToyWritePlugin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS HelloToyWritePlugin
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
