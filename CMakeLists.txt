# Set up the project.
cmake_minimum_required( VERSION 3.1 )
project( "GeoModelTools" VERSION 1.0.0 LANGUAGES CXX )

add_subdirectory(XMLParser)
add_subdirectory(JSONParser)
add_subdirectory(ExpressionEvaluator)
add_subdirectory(GMCAT)

install(EXPORT JSONParser-export FILE GeoModelTools-JSONParser.cmake DESTINATION lib/cmake/GeoModelTools)
install(EXPORT XMLParser-export FILE GeoModelTools-XMLParser.cmake DESTINATION lib/cmake/GeoModelTools)
install(EXPORT ExpressionEvaluator-export FILE GeoModelTools-ExpressionEvaluator.cmake DESTINATION lib/cmake/GeoModelTools)

install(FILES cmake/GeoModelToolsConfig.cmake DESTINATION lib/cmake/GeoModelTools)

