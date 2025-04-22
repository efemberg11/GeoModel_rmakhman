#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GeoModelIO::GeoModelDBManager" for configuration "Release"
set_property(TARGET GeoModelIO::GeoModelDBManager APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelIO::GeoModelDBManager PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "GeoModelCore::GeoModelHelpers;GeoModelCore::GeoModelKernel"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModelDBManager.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModelDBManager.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelIO::GeoModelDBManager )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelIO::GeoModelDBManager "${_IMPORT_PREFIX}/lib/libGeoModelDBManager.so.6.9.0" )

# Import target "GeoModelIO::TFPersistification" for configuration "Release"
set_property(TARGET GeoModelIO::TFPersistification APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelIO::TFPersistification PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTFPersistification.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libTFPersistification.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelIO::TFPersistification )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelIO::TFPersistification "${_IMPORT_PREFIX}/lib/libTFPersistification.so.6.9.0" )

# Import target "GeoModelIO::GeoModelRead" for configuration "Release"
set_property(TARGET GeoModelIO::GeoModelRead APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelIO::GeoModelRead PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModelRead.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModelRead.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelIO::GeoModelRead )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelIO::GeoModelRead "${_IMPORT_PREFIX}/lib/libGeoModelRead.so.6.9.0" )

# Import target "GeoModelIO::GeoModelWrite" for configuration "Release"
set_property(TARGET GeoModelIO::GeoModelWrite APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelIO::GeoModelWrite PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModelWrite.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModelWrite.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelIO::GeoModelWrite )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelIO::GeoModelWrite "${_IMPORT_PREFIX}/lib/libGeoModelWrite.so.6.9.0" )

# Import target "GeoModelIO::GeoModelIOHelpers" for configuration "Release"
set_property(TARGET GeoModelIO::GeoModelIOHelpers APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GeoModelIO::GeoModelIOHelpers PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGeoModelIOHelpers.so.6.9.0"
  IMPORTED_SONAME_RELEASE "libGeoModelIOHelpers.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS GeoModelIO::GeoModelIOHelpers )
list(APPEND _IMPORT_CHECK_FILES_FOR_GeoModelIO::GeoModelIOHelpers "${_IMPORT_PREFIX}/lib/libGeoModelIOHelpers.so.6.9.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
