cmake_minimum_required(VERSION 3.5)

project(fsl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
#find_package(nlohmann_json REQUIRED)



file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB uis ${PROJECT_SOURCE_DIR}/src/*.ui)
set(PROJECT_SOURCES ${sources} ${headers}  ${uis} )


add_executable(fsl ${PROJECT_SOURCES} )

target_link_libraries(fsl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


if( GEOMODEL_USE_BUILTIN_JSON )
   add_dependencies( GXClashPointSystems JSONExt )
else()
   target_link_libraries(fsl PRIVATE nlohmann_json::nlohmann_json)
endif()

# Install the executable.
install( TARGETS fsl
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )

