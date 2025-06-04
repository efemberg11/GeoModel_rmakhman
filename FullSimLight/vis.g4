# Macro file for the visualization setting in the initialization phase 
# of the spdKosoyGeomodel
#

# Specify a viewer, e.g., /vis/open OGL, or allow a system choice:
/vis/open 

# Disable auto refresh and quieten vis messages whilst scene and
# trajectories are established:
/vis/viewer/set/autoRefresh false
/vis/verbose errors
#
# Draw geometry:
/vis/viewer/set/specialMeshRendering
/vis/viewer/set/specialMeshRenderingOption surfaces
/vis/drawVolume

/vis/viewer/set/style surface
/vis/viewer/set/hiddenMarker true
/vis/viewer/set/viewpointThetaPhi 120 150
#
# Re-establish auto refreshing and verbosity:
/vis/viewer/set/autoRefresh true
/vis/verbose warnings
/vis/viewer/set/viewpointVector 0 0 1
/vis/viewer/refresh

/vis/set/touchable WorldLog 0 T1Log 16969
/vis/touchable/set/colour 0 1 0 1
/vis/viewer/refresh


/vis/set/touchable WorldLog 0 TRedLog 16969
/vis/touchable/set/colour 1 0 0 1
/vis/viewer/refresh


/vis/set/touchable WorldLog 0 TBlueLog 16969
/vis/touchable/set/colour 0 0 1 1



/vis/viewer/set/targetPoint -0 0 0 cm

/vis/scene/add/axes 0 0 0 1 m

/vis/viewer/set/lineSegmentsPerCircle 10