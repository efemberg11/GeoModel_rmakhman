# Install script for directory: /Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/mariba/Atlas/standaloneGeo2G4/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/build/libGeoSpecialShapes.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoSpecialShapes.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoSpecialShapes.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/mariba/Atlas/standaloneGeo2G4/install/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoSpecialShapes.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoSpecialShapes.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/GeoSpecialShapes" TYPE FILE FILES
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/GeoSpecialShapes/LArCustomShape.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/GeoSpecialShapes/LArGeoCheckerDict.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/GeoSpecialShapes/LArWheelCalculator.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/GeoSpecialShapes/LArWheelCalculatorEnums.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/GeoSpecialShapes/sincos.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorFactory.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorSaggingOff.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorSaggingOn.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/FanCalculatorFactory.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/IDistanceCalculator.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/IFanCalculator.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/ModuleFanCalculator.h"
    "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/src/LArWheelCalculator_Impl/WheelFanCalculator.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/mariba/Atlas/standaloneGeo2G4/GeoModelG4/GeoSpecialShapes/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
