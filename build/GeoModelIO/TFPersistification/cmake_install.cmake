# Install script for directory: /home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/rmakhman/dev_geomodel/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so.6.9.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification/libTFPersistification.so.6.9.0"
    "/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification/libTFPersistification.so.6"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so.6.9.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification/libTFPersistification.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTFPersistification.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/TFPersistification" TYPE FILE FILES
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/ACosIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/ASinIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/ATanIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/AbsIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/ArrayFunctionIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/CosIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/FixedConstantIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/GenFunctionInterpreter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/GenFunctionPersistifier.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/GenFunctionReader.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/GenFunctionRecorder.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/GenfunIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/ModIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/RectangularIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/SinIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/SqrtIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/SquareIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/TanIO.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/TransFunctionInterpreter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/TransFunctionPersistifier.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/TransFunctionReader.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelIO/TFPersistification/TFPersistification/TransFunctionRecorder.h"
    )
endif()

