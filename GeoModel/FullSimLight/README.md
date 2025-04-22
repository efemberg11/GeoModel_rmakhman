 FullSimLight Project
 ==========================

FullSimLight project consists of different tools based on [Geant4](https://geant4.web.cern.ch) toolkit, that can be run on multiple geometries:

- fullSimLight: a Geant4 based light simulation (geometry, transport in magnetic field and basic physics scoring)
- gmclash: a tool that runs clash detection on your input geometry, producing a json file report
- gmgeantino: a tool to generate geantino maps from your input geometry
- gmmasscalc: tool that calculates the inclusive and exclusive mass of an input geometry
- gm2gdml: a tool to convert geometries and dump them in gdml format.

The supported geometry formats are SQLite (.db), GDML (.gdml) and dual-use plugins (.dylib/.so) as the ones described in the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins). The ATLAS detector geometry description can be imported via a SQLite or a GDML file. If the ATLAS magnetic field map is available to the user, it can be used in fullSimLight.

For updated documentation on how to install and run fullsimlight please refer to the main GeoModel documentation [webpage](https://geomodel.web.cern.ch/home/fullsimlight/fullsimlight/)
