cmake_minimum_required(VERSION 3.16...3.26)

# Compile with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# Find the needed dependencies, when building individually
  message (${CMAKE_SOURCE_DIR})
  message (${PROJECT_SOURCE_DIR})

if ( CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR ) # when buildingindividually
   find_package( GeoModelCore REQUIRED  )
endif()


# Find the header and source files.
file( GLOB SOURCES src/*.cxx )

add_library( SurfAnnulusDemo SHARED ${SOURCES} )
target_link_libraries( SurfAnnulusDemo PUBLIC GeoModelCore::GeoModelKernel GeoModelCore::GeoModelHelpers)

source_group( "src" FILES ${SOURCES} )

set_target_properties( SurfAnnulusDemo PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS SurfAnnulusDemo
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
