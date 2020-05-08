
#
# Check for options to switch various optional functionalities
# in the source code
#
# This file sets up
#
#   GEOMODELREAD_TIMING
#   GEOMODELREAD_DEBUG
#   GEOMODELREAD_DEEP_DEBUG
#
#
# The options can be specified at configuration time by using, e.g.:
#    `cmake -DGEOMODELREAD_TIMING=ON`
# on the command line.
#

IF ( DEFINED GEOMODELREAD_TIMING )
  message( STATUS "You asked to print timing information... (`-DGEOMODELREAD_TIMING=${GEOMODELREAD_TIMING}`)")
  ADD_DEFINITIONS(-DGEOMODELREAD_TIMING)
ENDIF()
IF ( DEFINED GEOMODELREAD_DEBUG )
  message( STATUS "You asked to print debug information... (`-DGEOMODELREAD_DEBUG=${GEOMODELREAD_DEBUG}`)")
  ADD_DEFINITIONS(-DGEOMODELREAD_DEBUG)
ENDIF()
IF ( DEFINED GEOMODELREAD_DEEP_DEBUG )
  message( STATUS "You asked to print verbose debug information... (`-DGEOMODELREAD_DEEP_DEBUG=${GEOMODELREAD_DEEP_DEBUG}`)")
  ADD_DEFINITIONS(-DGEOMODELREAD_DEEP_DEBUG)
ENDIF()
