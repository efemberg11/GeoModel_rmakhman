# GeoModelPlugins

This repository contains plugins that create GeoModel geometries.

## Build from source.

To build  plugins for the GeometryExplorer <https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization>:
 
* Install the GeoModelCore development kit (quick method):

```bash
sudo add-apt-repository ppa:kaktusjoe/geomodel
sudo apt-get update
sudo apt install geomodel-core-dev
```

* Install the GeoModelCore development kit (from source):

      follow instructions at <https://gitlab.cern.ch/GeoModelDev/GeoModelCore>

* Build and install the plugins from source

Make sure you have built and installed the following packages:

- GeoModelCore (from GeoModelDev)
- GeoModelTools (from GeoModelDev)
- GeoModelDataManagers (GeoModelATLAS)

After that proceed with building the plugins and installing them into ${PLUGIN_INSTALL_DIR} as follows:

```bash
git clone https://gitlab.cern.ch/atlas/GeoModelPlugins
mkdir build_plugins
cd build_plugins
cmake -DCMAKE_INSTALL_PREFIX=${PLUGIN_INSTALL_DIR} ../GeoModelPlugins
make -j4
make instal
```
      
* Your plugins are ready!  Run them with gmex (see <https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization> )

## ToyGeometryPlugin

ToyGeometryPlugin reads material definitions form XML files, which are found in the ToyGeometryPlugin/data directory. In order to access these files - elements.xml and material.xml - at runtime, they need to be placed in the run directory of gmex. This is considered a temporary solution. In the long term we need to design a special mechanism allowing GeoModel applications (e.g. plugins) to access input XML files.

## LAr Plugin

LAr plugin builds GeoModel description of the ATLAS Liquid Argon Calorimeter. For more details about this plugin see LArPlugin/README.md