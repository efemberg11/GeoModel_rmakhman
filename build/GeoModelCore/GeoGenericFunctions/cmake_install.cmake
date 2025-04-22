# Install script for directory: /home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so.6.9.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions/libGeoGenericFunctions.so.6.9.0"
    "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions/libGeoGenericFunctions.so.6"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so.6.9.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions/libGeoGenericFunctions.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoGenericFunctions.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/GeoGenericFunctions" TYPE FILE FILES
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ACos.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ASin.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ATan.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Abs.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Abs.icc"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/AbsFunction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/AbsParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Argument.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ArrayFunction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstMinusFunction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstMinusParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstOverFunction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstOverParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstPlusFunction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstPlusParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstTimesFunction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ConstTimesParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Cos.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FixedConstant.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionComposition.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionDifference.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionDirectProduct.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionNegation.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionNoop.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionNumDeriv.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionPlusParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionProduct.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionQuotient.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionSum.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/FunctionTimesParameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/KVector.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Mod.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Parameter.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ParameterComposition.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ParameterDifference.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ParameterNegation.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ParameterProduct.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ParameterQuotient.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/ParameterSum.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Pi.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Power.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Rectangular.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Sigma.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Sin.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Sqrt.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Square.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Tan.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Theta.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoGenericFunctions/GeoGenericFunctions/Variable.h"
    )
endif()

