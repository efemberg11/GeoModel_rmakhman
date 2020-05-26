set( CMAKE_CXX_FLAGS "-O0" )
project( "GeoModelPlugins-Test" LANGUAGES CXX )

find_package( GeoModelCore REQUIRED )
find_package( GeoModelDataManagers REQUIRED )

add_executable ( toytest src/toytest.cxx )
target_link_libraries ( toytest GeoModelCore::GeoModelKernel GeoXmlMatManager )

install( TARGETS toytest DESTINATION bin )
