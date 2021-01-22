# GeoModelATLAS

The GeoModelATLAS repository gathers all GeoModel software packages that are specific to the ATLAS experiment.

## Dependencies

### External Dependencies

To build GeoModelATLAS you need a few third-party dependencies: 

* `Eigen3` 
* `Xerces-C`
* `ZLIB`

Usually, you can install them on your system by using the default package manager for your folder. For instance, if not installed already, on macOS you can install them  by using [brew](https://brew.sh); for example, with: `brew install eigen xerces-c`. 

A note: a proper version of `ZLIB` is usually installed together with the operating system, so there is usually no need to install another version of it.


### GeoModel 

To build `GeoModelATLAS`, the latest version of `GeoModel` must be compiled and installed (pre-compiled packages have not been updated yet)

```
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModel.git
mkdir build_gm 
cd build_gm
cmake -DGEOMODEL_BUILD_TOOLS=1 -DGEOMODEL_BUILD_VISUALIZATION=1 ../GeoModel 
make -j4
make install
cd ..
```

The commands above will build the latest version of `GeoModelCore`, `GeoModelIO`, `GeoModelTools`, and `GeoModelVisualization` (`gmex`). 

By default, those will be installed in a system folder (*e.g.*, in `/usr/local` on macOS). However, if you want to have them installed locally, for example if you have other versions installed on your system, you can install them locally by using the CMake option `-DCMAKE_INSTALL_PREFIX=../install`.

**Please note** that if you have other versions of `GeoModel` installed in default system folders, or if you installed them with package managers (like `brew` on macOS), CMake will try to use them at first. So you'll probably get errors while trying to compile `GeoModelATLAS`. Please be sure you pick the right, latest version of `GeoModel` when compiling `GeoModelATLAS`.


## Build GeoModelATLAS

### Configuration 

The CMake setup has been configured to offer the developers several build options, which are briefly summarized here below.

**Please, also take a look at additional, package-specific instructions contained in the packages' README files.**

#### DATAMANAGERS 

| Default: | `ON` |


The `GEOMODELATLAS_BUILD_DATAMANAGERS` option builds `GeoModelDataManagers` only. This is set as `ON` by default, so `GeoModelDataManagers` is built even if no build options are given to the `cmake` command. 


```
cmake -DGEOMODELATLAS_BUILD_DATAMANAGERS=1 ../GeoModelATLAS
```

#### AGDD 

| Default: | `OFF` |

The `GEOMODELATLAS_BUILD_AGDD` option builds the `AGDD` and the `GeoModelDataManagers` packages:

```
cmake -DGEOMODELATLAS_BUILD_AGDD=1 ../GeoModelATLAS
```

#### MUONDD 

| Default: | `OFF` |

The `GEOMODELATLAS_BUILD_MUONDD` option builds the `MuonDD`, `AGDD`, and `GeoModelDataManagers` packages:

```
cmake -DGEOMODELATLAS_BUILD_MUONDD=1 ../GeoModelATLAS
```

#### GEOMODELXML 

| Default: | `OFF` |


The `GEOMODELATLAS_BUILD_GEOMODELXML` builds the `GeoModelXML` package only:

```
cmake -DGEOMODELATLAS_BUILD_GEOMODELXML=1  ../GeoModelATLAS
```

#### ALL 

| Default: | `OFF` | 

By enabling the `GEOMODELATLAS_BUILD_ALL` option, all the sub-projects contained in GeoModelATLAS are built. You can enable it at configuration time by using:

```
cmake -DGEOMODELATLAS_BUILD_ALL=1 ../GeoModelATLAS
```


### Build 

Once you have decided the configuration you need, you can build by following the instructions below. 

If no options have been given to `cmake`, it will build the packages which are set as `ON` by default.

As an example, let's say you want to build all the packages, then you can build GeoModelATLAS with:

```
mkdir build
cd build 
cmake -DGEOMODELATLAS_BUILD_ALL=1 ../GeoModelATLAS
make -j4
make install
```

#### Local installation

By default, the `make install` command installs the executables, libraries, and headers in a system folder (*e.g.*, `/usr/local/lib`). If instead you would like to install them in a local folder (for example, in order to be able to install and test multiple versions of GeoModelATLAS packages), you can pass an extra CMake option as here below:

```
cmake -DGEOMODELATLAS_BUILD_ALL=1 -DCMAKE_INSTALL_PREFIX=../install ../GeoModelATLAS
```

In the example above, `make install` will install everything in the folder `../install`, with the path relative to the `build` folder.


## LICENSE

Following the ATLAS Experiment, CERN, and HSF reccomandations, GeoModelATLAS is released under an Apache 2.0 license.


## Contacts

For all questions, you can write at [geomodel-developers@cern.ch](mailto:geomodel-developers@cern.ch)



