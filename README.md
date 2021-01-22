# GeoModelPlugins

This repository contains plugins that create GeoModel geometries.

## Build from source.

### Build the whole GeoModel, GeoModelATLAS, GeoModelPlugins software stack

#### Dependencies


##### Build GeoModel

These instructions assume you build GeoModel from sources; however, if you're not interested in developing the GeoModel code itself, you can simply install it by using, on supported platforms, a package manager. Refer to the main documentation for that: https://geomodel.web.cern.ch/home/start/install/

As external dependencies, you need at least Eigen3, nlohmann_josn, Xerces-C on your computer. Plus, Coin, SoQt, and Qt5 if you want to build the visualization tools. Refer to the GeoModel documentation for more details: https://geomodel.web.cern.ch/home/dev/

After that, you can build GeoModel:


```
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModel.git

mkdir build_gm
cd build_gm

cmake -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_BUILD_TOOLS=1 -DGEOMODEL_BUILD_VISUALIZATION=1 ../GeoModel
```

You should see this output message on your screen:

```
-- -----
-- Building the following 3 packages: GeoModelCore, GeoModelIO, GeoModelTools, GeoModelVisualization
-- -----
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/rbianchi/code_work_local/atlas/GeoModelDev/test_geomodelplugins_build_geomodelatlas_monorepo_13Jan2021/b_gm
```

Then, you can compile GeoModel:

```
make -j4
make install
cd ..
```



##### Build GeoModelATLAS


```
git clone ssh://git@gitlab.cern.ch:7999/atlas/geomodelatlas/GeoModelATLAS.git
mkdir build_gmatlas
cd build_gmatlas

cmake -DCMAKE_INSTALL_PREFIX=../install -DGEOMODELATLAS_BUILD_ALL=1 ../GeoModelATLAS
make -j4
make install
cd ..
```

#### Build GeoModelPlugins

```
git ckone ssh://git@gitlab.cern.ch:7999/atlas/GeoModelPlugins.git
mkdir build_plugins
cd build_plugins

cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelPlugins
make -j4
make install
```



* Your plugins are ready!  Run them with gmex (see <https://gitlab.cern.ch/GeoModelDev/GeoModel/GeoModelVisualization> )

## ToyGeometryPlugin

ToyGeometryPlugin reads material definitions form XML files, which are found in the ToyGeometryPlugin/data directory. In order to access these files - elements.xml and material.xml - at runtime, they need to be placed in the run directory of gmex. This is considered a temporary solution. In the long term we need to design a special mechanism allowing GeoModel applications (e.g. plugins) to access input XML files.

## LAr Plugin

LAr plugin builds GeoModel description of the ATLAS Liquid Argon Calorimeter. For more details about this plugin see LArPlugin/README.md