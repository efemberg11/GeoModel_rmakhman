# Install script for directory: /home/rmakhman/dev_geomodel/GeoModel/FullSimLight

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/rmakhman/dev_geomodel/build/FullSimLight/Plugins/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/rmakhman/dev_geomodel/build/bin/fullSimLight")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoModel2G4:/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoMaterial2G4:/home/rmakhman/geant4/Geant4-install/lib:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelRead:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelWrite:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelDBManager:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelHelpers:/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fullSimLight")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/rmakhman/dev_geomodel/build/bin/gmclash")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoModel2G4:/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoMaterial2G4:/home/rmakhman/geant4/Geant4-install/lib:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelRead:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelWrite:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelDBManager:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelHelpers:/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmclash")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/rmakhman/dev_geomodel/build/bin/gmmasscalc")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoModel2G4:/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoMaterial2G4:/home/rmakhman/geant4/Geant4-install/lib:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelRead:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelWrite:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelDBManager:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelHelpers:/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmmasscalc")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/rmakhman/dev_geomodel/build/bin/fillHistogramExample")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample"
         OLD_RPATH "/home/rmakhman/geant4/Geant4-install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fillHistogramExample")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/rmakhman/dev_geomodel/build/bin/gmgeantino")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoModel2G4:/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoMaterial2G4:/home/rmakhman/geant4/Geant4-install/lib:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelRead:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelWrite:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelDBManager:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelHelpers:/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmgeantino")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/rmakhman/dev_geomodel/build/bin/gm2gdml")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoModel2G4:/home/rmakhman/dev_geomodel/build/GeoModelG4/GeoMaterial2G4:/home/rmakhman/geant4/Geant4-install/lib:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelRead:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelWrite:/home/rmakhman/dev_geomodel/build/GeoModelIO/GeoModelDBManager:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelHelpers:/home/rmakhman/dev_geomodel/build/GeoModelIO/TFPersistification:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel:/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gm2gdml")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/FullSimLight" TYPE FILE FILES
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino_Pixel.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino_SCT.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino_TRT.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino_LAr.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino_Tile.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/geantino_Muon.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/macro.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/pythia.g4"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/drawMagField.C"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/drawGeantinoMaps.C"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight" TYPE FILE FILES "/home/rmakhman/dev_geomodel/build/CMakeFiles/FullSimLightConfigVersion.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight/FullSimLightTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight/FullSimLightTargets.cmake"
         "/home/rmakhman/dev_geomodel/build/FullSimLight/CMakeFiles/Export/lib/cmake/FullSimLight/FullSimLightTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight/FullSimLightTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight/FullSimLightTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight" TYPE FILE FILES "/home/rmakhman/dev_geomodel/build/FullSimLight/CMakeFiles/Export/lib/cmake/FullSimLight/FullSimLightTargets.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/FullSimLight" TYPE FILE FILES "/home/rmakhman/dev_geomodel/build/CMakeFiles/FullSimLightConfig.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FullSimLight" TYPE FILE FILES
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/FullSimLight/FSLSensitiveDetectorPlugin.h"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/FullSimLight/MagFieldPlugin.h"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/FullSimLight/FSLPhysicsListPlugin.h"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/FullSimLight/FSLUserActionPlugin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/rmakhman/dev_geomodel/install/man/man1/fullSimLight.1;/home/rmakhman/dev_geomodel/install/man/man1/gmclash.1;/home/rmakhman/dev_geomodel/install/man/man1/gmgeantino.1;/home/rmakhman/dev_geomodel/install/man/man1/gmmasscalc.1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/rmakhman/dev_geomodel/install/man/man1" TYPE FILE FILES
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/man/man1/fullSimLight.1"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/man/man1/gmclash.1"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/man/man1/gmgeantino.1"
    "/home/rmakhman/dev_geomodel/GeoModel/FullSimLight/man/man1/gmmasscalc.1"
    )
endif()

