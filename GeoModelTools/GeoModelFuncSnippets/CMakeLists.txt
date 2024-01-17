# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Find the header and source files.
file( GLOB SOURCES src/*.cxx )
file( GLOB HEADERS GeoModelFuncSnippets/*.h GeoModelFuncSnippets/*.icc )

# Create the library.
add_library( GeoModelFuncSnippets SHARED ${HEADERS} ${SOURCES} )

target_link_libraries( GeoModelFuncSnippets PRIVATE GeoModelCore::GeoModelKernel )


target_include_directories( GeoModelFuncSnippets PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
source_group( "GeoModelFuncSnippets" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelFuncSnippets PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Set up an alias with the same name that you would get by "finding" a pre-built
# version of the library.
add_library( GeoModelTools::GeoModelFuncSnippets ALIAS GeoModelFuncSnippets )

# Install the library.
install(TARGETS GeoModelFuncSnippets
    EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelFuncSnippets
            COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelFuncSnippets
   COMPONENT Development )

file(GLOB_RECURSE files "tests/*.cxx")
foreach(_exeFile ${files})
  get_filename_component(_theExec ${_exeFile} NAME_WE)
  get_filename_component(_theLoc ${_exeFile} DIRECTORY)
  
  if(${_theLoc} MATCHES "DoNotBuild")
    continue()
  endif() 
  
  add_executable(${_theExec} ${_exeFile})
  target_link_libraries( ${_theExec} GeoModelTools::GeoModelFuncSnippets GeoModelCore::GeoModelKernel)
  add_test(NAME ${_theExec}
           COMMAND ${_theExec})

endforeach()
