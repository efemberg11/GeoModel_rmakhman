# ATLAS Extensions

FullSimLight is an experiment agnostic detector simulation tool that has been implemented and is mantained by the ATLAS collaboration. All the ATLAS specific customizations are available at the [ATLAS Extensions](https://gitlab.cern.ch/atlas/geomodelatlas/ATLASExtensions) repository that is linked to the GeoModel repository as a git submodule. In order to have the corresponding code cloned inside the GeoModel repository the user would need to clone recursively the main repository. So either:

If you still didn't clone the repository you can clone it recursively:

```c++
git clone --recursive https://gitlab.cern.ch/GeoModelDev/GeoModel.git
```
Alternatively, if you already cloned the GeoModel repository, it will be sufficient to type the following commands from the main repository folder:

```c++
git submodule init
git submodule update
```

A flag is available to build the ATLAS extentions (by default OFF) and it should be turned ON in order to build all the extensions:

```c++
cmake ../ -DGEOMODEL_BUILD_ATLASEXTENSIONS=ON
```

A configuration file {{atlas-conf.json}} with all the ATLAS specific customizations is provided with the ATLASExtensions and will be installed under {{<install>/share/FullSimLight/ATLAS}}. In order to run a Geant4 ATLAS with your favourite geometry tag you only have to run the following:
```c++
./fullSimLight  -g geometry.db -c atlas-conf.json
```

## ATLAS Geometry File
Some ATLAS geometry SQLite files are available at the following [link](https://geomodel.web.cern.ch/atlas-geometry-data/), only worrking under CERN SSO.
They describe the whole geometry tree of different tags used during the different LHC runs. You can select your favourite one and use it with fullsimlight!

## ATLAS Magnetic Field Map and plugin
<!---->
This folder contains the ATLAS magnetic field map implementation. It uses a magnetic field map data file that is available at the following [link](https://geomodel.web.cern.ch/atlas-magnetic-field/) and can be downloaded with wget:
```c++
wget https://geomodel.web.cern.ch/atlas-magnetic-field/bmagatlas_09_fullAsym20400.data
```
## LAr Custom Solid Extension
<!---->
This folder contains the LArCustomSolid Extension that describes the ATLAS LAr EMEC custom shape. The LAr EMEC custom shape extension is build starting from a copy of the corresponding code in the Athena repository.
To keep the code of the LArCustomSolid Extention synchronized with the latest available version in the Athena repository, just run the autonomous-lar.sh script.
