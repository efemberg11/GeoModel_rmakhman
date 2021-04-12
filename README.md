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

#### PLUGINS

| Default: | `OFF` |


The `GEOMODELATLAS_BUILD_PLUGINS` builds the `GeoModelPlugins` and the `GeoModelDataManagers` packages:

```
cmake -DGEOMODELATLAS_BUILD_PLUGINS=1  ../GeoModelATLAS
```


#### BASE 

| Default: | `OFF` | 

By enabling the `GEOMODELATLAS_BUILD_BASE` option, all the sub-projects contained in `GeoModelATLAS` except `GeoModelPlugins` are built. You can enable it at configuration time by using:

```
cmake -DGEOMODELATLAS_BUILD_BASE=1 ../GeoModelATLAS
```


#### ALL 

| Default: | `OFF` | 

By enabling the `GEOMODELATLAS_BUILD_ALL` option, all the sub-projects contained in `GeoModelATLAS` are built (`GeoModelPlugins` included). You can enable it at configuration time by using:

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

## Docker image

As an alternative to locally installing and / or building `GeoModel` and `GeoModelATLAS`, there is also a `[docker](https://www.docker.com/)` image available for this project.
It includes all the required dependencies and delivers an 'out of the box' experience. 

The available images are listed in the project's [container registry](https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelATLAS/container_registry). 

### Getting set 
To be able to run a container based on this image: 

* Install [docker](https://www.docker.com/) on your system in case it is not yet available. You might also be able to use the `singularity` installation supplied via `cvmfs`, but this has not yet been tested. 
* If you haven't done so in the past, you will need to log in to the CERN gitlab registry once: 
```
docker login gitlab-registry.cern.ch
```
* Then, you can pull the `geomodelatlas` image: 
```
docker pull gitlab-registry.cern.ch/atlas/geomodelatlas/geomodelatlas:master
``` 
This will make a local image available on your computer that you can use to launch containers. 
To list the available images, you can use `docker images`. 

### Running a container 
* You can now start up a container using a single command. Please see the [reference](https://docs.docker.com/engine/reference/run/) for the `docker run` command for a detailed documentation of the available options. 

We will want to start an interactive shell, make parts of our file system available (to be able to work with our local geometry files), and ensure that X-forwarding is possible so that we can display graphical applications. 

The details of the prompt will change slightly depending on your OS and preferences, the general format is: 
```
docker run -w ${PWD} -v ${PWD}:${PWD} -e DISPLAY=$DISPLAY -it gitlab-registry.cern.ch/atlas/geomodelatlas/geomodelatlas:master
```

The `-v` option allows you to make file system paths available within the container. The syntax is `-v <local path>:<path to make it available on>`. 

In my example above, I assume I am in the working directory within which all my sources / geometry files are located. 

With `-e`, you can specify environment variables. The `DISPLAY` variable is definitely needed. 

For users of the [GeoModelXML plugin](https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelATLAS/-/tree/master/GeoModelXML), it can be useful to also set `-e LANG=en_US.UTF-8`.

#### Note for MacOS:

On MacOS, you need to have `XQuartz` installed (also in case you compile from source) and in the `XQuartz` security settings, you need to allow network client connections. You need to specify the `DISPLAY` variable when running docker and note that you need to pass the docker image ID (you find it by `docker images`):
```
xhost + "$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')"
docker run -e DISPLAY="$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')":0  -w ${PWD} -v ${PWD}:${PWD} -it <image ID> 
```

#### Note for Windows 10 with Windows Subsystem for Linux (WSL):

In case you are using Windows 10 with the Windows Subsystem for Linux (WSL), you might be interested in this Twiki in general: https://twiki.cern.ch/twiki/bin/view/Sandbox/RunningATLASOnWSL2
You'll need to install an X server (for example, [VcXsrv](https://sourceforge.net/projects/vcxsrv/)), as Windows doesn't ship with one. 
In addition, you will need to add exceptions to your windows firewall to allow connections to your X-server program. 
When running docker, you then have to set the `DISPLAY` variable via
```
docker run -e DISPLAY="$(grep nameserver /etc/resolv.conf  | awk '{print $2}'):0.0" -w ${PWD} -v ${PWD}:${PWD} -it gitlab-registry.cern.ch/atlas/geomodelatlas/geomodelatlas:master
```

### Where to find the libraries within the container? 
If you have the container running and need to find libraries (for example the GMX plugin), you can find them under `/usr/local/lib`.

For example, to run `gmex` with the `GeoModelXML` plugin, you could call 
```
GMX_FILES=<path_to_my_gmx_file> gmex /usr/local/lib/libGMXPlugin.so
```

## LICENSE

Following the ATLAS Experiment, CERN, and HSF reccomandations, GeoModelATLAS is released under an Apache 2.0 license.


## Contacts

For all questions, you can write at [geomodel-developers@cern.ch](mailto:geomodel-developers@cern.ch)



