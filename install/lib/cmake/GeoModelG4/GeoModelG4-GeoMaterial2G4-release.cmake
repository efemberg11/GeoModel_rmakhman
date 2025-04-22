#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GeoMaterial2G4" for configuration "Release"
set_property(TARGET GeoMaterial2G4 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoMaterial2G4 PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoMaterial2G4.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoMaterial2G4.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoMaterial2G4 )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoMaterial2G4 "${_IMPORT_PREFIX}/lib/libGeoMaterial2G4.so.6.9.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
