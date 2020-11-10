cmake_minimum_required( VERSION 3.1 )
project( "TRTGeoPlugin" VERSION 1.0.0 LANGUAGES CXX )

find_package( GeoModelCore REQUIRED )
find_package( GeoModelTools REQUIRED )
find_package( GeoModelDataManagers REQUIRED )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Find the header and source files.
file( GLOB SOURCES src/*.cxx)

add_library( TRTGeoPlugin SHARED ${SOURCES} )
target_link_libraries( TRTGeoPlugin PUBLIC GeoModelCore::GeoModelKernel GeoXmlInpManager GeoXmlMatManager GeoModelTools::GeoModelXMLParser) 
set_target_properties( TRTGeoPlugin PROPERTIES
		       VERSION ${PROJECT_VERSION}
		       SOVERSION ${PROJECT_VERSION_MAJOR} )

install( TARGETS TRTGeoPlugin
	 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	 COMPONENT Runtime
	 NAMELINK_SKIP )
