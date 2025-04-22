#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GeoModel2G4" for configuration "Release"
set_property(TARGET GeoModel2G4 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModel2G4 PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "GeoMaterial2G4;GeoModelCore::GeoModelKernel"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModel2G4.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModel2G4.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModel2G4 )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModel2G4 "${_IMPORT_PREFIX}/lib/libGeoModel2G4.so.6.9.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
