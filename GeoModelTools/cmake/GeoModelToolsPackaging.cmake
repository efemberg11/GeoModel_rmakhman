# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Set up the basic properties of the package(s).
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY
   "${PROJECT_NAME} - ${PROJECT_VERSION}" )
set( CPACK_PACKAGE_DESCRIPTION
   "Geometry model tool libraries" )
set( CPACK_PACKAGE_VERSION "${PROJECT_VERSION}" )
set( CPACK_PACKAGE_CONTACT "geomodel-developers@cern.ch" )

# Set up the readme and license for the package.
set( CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE )
set( CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README.md )
set( CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md )

# Set up the handling of the Runtime and Development components during
# the packaging.
include( CPackComponent )
set( CPACK_COMPONENTS_ALL Runtime Development )
set( CPACK_RPM_COMPONENT_INSTALL TRUE )
set( CPACK_DEB_COMPONENT_INSTALL TRUE )
set( CPACK_COMPONENTS_IGNORE_GROUPS TRUE )
cpack_add_component( Runtime
   DISPLAY_NAME "GeoModelTools libraries/executables"
   DESCRIPTION "Core runtime libraries/executables of GeoModelTools"
   REQUIRED )
cpack_add_component( Development
   DISPLAY_NAME "GeoModelTools development"
   DESCRIPTION "Development package for GeoModelTools"
   DEPENDS Runtime
   DISABLED )

# Include the main CPack configuration.
include( CPack )
