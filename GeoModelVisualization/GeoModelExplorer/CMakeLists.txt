# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

# Add the executable.
add_executable( gmex src/main.cxx )
target_link_libraries( gmex PRIVATE GXGui Qt5::Core )
target_compile_definitions( gmex PRIVATE
   GEOMODEL_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}" )
source_group( "src" FILES "src/main.cxx" )

# Install the executable.
install( TARGETS gmex
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )
