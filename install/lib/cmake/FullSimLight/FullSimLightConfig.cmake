# Include the exported configuration of FullSimLight.
get_filename_component( SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH )
include( ${SELF_DIR}/FullSimLightTargets.cmake )

# Set the version of the installed project.
set( FullSimLight_VERSION "6.9.0" )
set( FullSimLight_HAS_HEPMC3 OFF )
set( FullSimLight_HAS_Pythia OFF )

# Print some standard messages about the package being found.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( FullSimLight
   FOUND_VAR FullSimLight_FOUND
   REQUIRED_VARS SELF_DIR
   VERSION_VAR FullSimLight_VERSION )

# Clean up.
unset( SELF_DIR )
