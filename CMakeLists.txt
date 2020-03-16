# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelTools" VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory(GeoModelXMLParser)
add_subdirectory(GeoModelJSONParser)
add_subdirectory(ExpressionEvaluator)
add_subdirectory(GMCAT)

install(EXPORT GeoModelJSONParser-export FILE GeoModelTools-GeoModelJSONParser.cmake DESTINATION lib/cmake/GeoModelTools)
install(EXPORT GeoModelXMLParser-export FILE GeoModelTools-GeoModelXMLParser.cmake DESTINATION lib/cmake/GeoModelTools)
install(EXPORT ExpressionEvaluator-export FILE GeoModelTools-ExpressionEvaluator.cmake DESTINATION lib/cmake/GeoModelTools)

install(FILES cmake/GeoModelToolsConfig.cmake DESTINATION lib/cmake/GeoModelTools)

