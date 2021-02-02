# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration

cmake_minimum_required( VERSION 3.12 )
project( "LArGeoPlugin" )


#Set up the project. Check if we build it with GeoModel or individually
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # I am built as a top-level project.
    # Make the root module directory visible to CMake.
    list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )
    # get global GeoModel version
    include( GeoModelATLAS-version ) 
    # set the project, with the version taken from the GeoModel parent project
    project( "LArGeoPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
    # Define color codes for CMake messages
    include( cmake_colors_defs )
    # Warn the users about what they are doing
    message(STATUS "${BoldGreen}Building ${PROJECT_NAME} individually, as a top-level project.${ColourReset}")
    # Set default build and C++ options
    include( configure_cpp_options )
    set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
         "Framework finding behaviour on macOS" )
    # Find the base GeoModelATLAS packages, which must be installed on the target system already
    find_package( GeoModelDataManagers REQUIRED ${GeoModelATLAS_VERSION} )
else()
    # I am called from other project with add_subdirectory().
    message( STATUS "Building ${PROJECT_NAME} as part of the root GeoModelATLAS project.")
    # Set the project
    project( "LArGeoPlugin" VERSION ${GeoModelATLAS_VERSION} LANGUAGES CXX )
endif()

# Other project's dependencies.
# GeoModel dependencies
find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )

# Use the GNU install directory names.
include( GNUInstallDirs )


# Find the header and source files.
file( GLOB SOURCES plugin/*.cxx LArGeoUtils/src/*.cxx LArGeoBarrel/src/*.cxx LArGeoEndcap/src/*.cxx GeoSpecialShapes/src/*.cxx GeoSpecialShapes/src/LArWheelCalculator_Impl/*.cxx LArGeoHec/src/*.cxx LArGeoFcal/src/*.cxx)

# Define include directories
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/LArGeoUtils ${CMAKE_CURRENT_SOURCE_DIR}/LArGeoBarrel ${CMAKE_CURRENT_SOURCE_DIR}/LArGeoEndcap ${CMAKE_CURRENT_SOURCE_DIR}/GeoSpecialShapes ${CMAKE_CURRENT_SOURCE_DIR}/GeoSpecialShapes/src/LArWheelCalculator_Impl ${CMAKE_CURRENT_SOURCE_DIR}/LArGeoHec ${CMAKE_CURRENT_SOURCE_DIR}/LArGeoFcal )


add_library( LArGeoPlugin SHARED ${SOURCES} )
target_link_libraries( LArGeoPlugin PUBLIC GeoModelCore::GeoModelKernel GeoModelDataManagers::GeoXmlInpManager GeoModelDataManagers::GeoXmlMatManager GeoModelTools::GeoModelXMLParser) 
set_target_properties( LArGeoPlugin PROPERTIES
		       VERSION ${PROJECT_VERSION}
		       SOVERSION ${PROJECT_VERSION_MAJOR} )

install( TARGETS LArGeoPlugin
	 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	 COMPONENT Runtime
	 NAMELINK_SKIP )
