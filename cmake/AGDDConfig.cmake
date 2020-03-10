get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/AGDD-AGDDKernel.cmake)
include(${SELF_DIR}/AGDD-AGDDModel.cmake)
include(${SELF_DIR}/AGDD-AGDDHandlers.cmake)
include(${SELF_DIR}/AGDD-AGDDControl.cmake)

