
#
# Check for options to switch various optional functionalities
# in the source code
#
# This file sets up
#
#   GEOMODEL_IO_TIMING
#   GEOMODEL_IO_DEBUG
#   GEOMODEL_IO_DEEP_DEBUG
#
#
# The options can be specified at configuration time by using, e.g.:
#    `cmake -DGEOMODEL_IO_TIMING=ON`
# on the command line.
#

IF ( DEFINED GEOMODEL_IO_TIMING )
  message( STATUS "You asked to print timing information... (`-DGEOMODEL_IO_TIMING=${GEOMODEL_IO_TIMING}`)")
  ADD_DEFINITIONS(-DGEOMODEL_IO_TIMING)
ENDIF()
IF ( DEFINED GEOMODEL_IO_DEBUG )
  message( STATUS "You asked to print debug information... (`-DGEOMODEL_IO_DEBUG=${GEOMODEL_IO_DEBUG}`)")
  ADD_DEFINITIONS(-DGEOMODEL_IO_DEBUG)
ENDIF()
IF ( DEFINED GEOMODEL_IO_DEBUG_VERBOSE )
  message( STATUS "You asked to print verbose debug information... (`-DGEOMODEL_IO_DEBUG_VERBOSE=${GEOMODEL_IO_DEBUG_VERBOSE}`)")
  ADD_DEFINITIONS(-DGEOMODEL_IO_DEBUG_VERBOSE)
ENDIF()
