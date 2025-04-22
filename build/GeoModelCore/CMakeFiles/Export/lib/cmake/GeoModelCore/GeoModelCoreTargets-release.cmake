#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GeoModelCore::GeoGenericFunctions" for configuration "Release"
set_property(TARGET GeoModelCore::GeoGenericFunctions APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelCore::GeoGenericFunctions PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoGenericFunctions.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoGenericFunctions.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelCore::GeoGenericFunctions )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelCore::GeoGenericFunctions "${_IMPORT_PREFIX}/lib/libGeoGenericFunctions.so.6.9.0" )

# Import target "GeoModelCore::GeoModelKernel" for configuration "Release"
set_property(TARGET GeoModelCore::GeoModelKernel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelCore::GeoModelKernel PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModelKernel.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModelKernel.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelCore::GeoModelKernel )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelCore::GeoModelKernel "${_IMPORT_PREFIX}/lib/libGeoModelKernel.so.6.9.0" )

# Import target "GeoModelCore::GeoModelHelpers" for configuration "Release"
set_property(TARGET GeoModelCore::GeoModelHelpers APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelCore::GeoModelHelpers PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "GeoModelCore::GeoModelKernel"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModelHelpers.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModelHelpers.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelCore::GeoModelHelpers )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelCore::GeoModelHelpers "${_IMPORT_PREFIX}/lib/libGeoModelHelpers.so.6.9.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
