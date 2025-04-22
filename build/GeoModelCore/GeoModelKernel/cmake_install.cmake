# Install script for directory: /home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so.6.9.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel/libGeoModelKernel.so.6.9.0"
    "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel/libGeoModelKernel.so.6"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so.6.9.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoModelKernel/libGeoModelKernel.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so"
         OLD_RPATH "/home/rmakhman/dev_geomodel/build/GeoModelCore/GeoGenericFunctions:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGeoModelKernel.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/GeoModelKernel" TYPE FILE FILES
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/CellBinning.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/CellPartitioning.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoAccessSurfaceAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoAccessVolAndSTAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoAccessVolumeAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoAlignableTransform.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoAnnulusSurface.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoBox.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoClearAbsPosAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoCons.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoCountSurfaceAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoCountVolAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoCountVolAndSTAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoCutVolAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoDefinitions.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoDefinitions.icc"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoDiamondSurface.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoElement.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoEllipticalTube.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoFacet.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoFullPhysVol.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoGenericTrap.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoGeometryPluginLoader.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoGraphNode.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoIdentifierTag.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoIntrusivePtr.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoLogVol.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoMaterial.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoNameTag.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoNodeAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoNodePath.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoNodePositioning.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPara.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPcon.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPerfUtils.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPgon.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPhysVol.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPlacement.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPluginLoader.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPolyhedrizeAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPolyhedron.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPrintGraphAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPublisher.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoPublisher.tpp"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoRectSurface.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoSerialDenominator.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoSerialIdentifier.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoSerialTransformer.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShape.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShapeAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShapeIntersection.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShapePath.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShapeShift.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShapeSubtraction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoShapeUnion.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoSimplePolygonBrep.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoSurfaceCursor.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTessellatedSolid.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTorus.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTransform.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTrap.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTrapezoidSurface.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTraversalState.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTrd.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTube.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTubs.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoTwistedTrap.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoUnidentifiedShape.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoUtilFunctions.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVAlignmentStore.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVDetectorElement.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVDetectorFactory.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVDetectorManager.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVFullPhysVol.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVG4ExtensionSolid.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVGeometryPlugin.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVPhysVol.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVSurface.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVSurfaceShape.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVolumeAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVolumeCursor.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoVolumeTagCatalog.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeoXF.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/GeometryMap.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/Path.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/PersistifierFwd.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/RCBase.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/TemplateVolAction.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/TinyHepTransform.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/Units.h"
    "/home/rmakhman/dev_geomodel/GeoModel/GeoModelCore/GeoModelKernel/GeoModelKernel/throwExcept.h"
    )
endif()

