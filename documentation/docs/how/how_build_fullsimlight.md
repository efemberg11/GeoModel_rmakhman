
# How to build FullSimLight

# Standard build

FullSimLight can be built as part of the GeoModel build, by enabling the related configuration flag `GEOMODEL_BUILD_FULLSIMLIGHT`:

```bash
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 ../GeoModel
make -j
make install
```

That will compile the base GeoModel packages (*i.e.*, `GeoModelCore`, `GeoModelIO`, and `GeoModelTools`), the GeoModel<->Geant4 interface package `GeoModelG4`, and the application `FullSimLight`.

This will install all the base libraries and the `fullSimLight` executable in the standard installation path (*e.g.*, `/usr/local` on macOS). If you want to have a local installation, see below.

# Local installation

You can install FullSimLight in a local folder as well. This let you remove everything by simply deleting the local folder, and lets you have multiple versions of the packages installed.

You can install iFullSimLight locally by using the `CMAKE_INSTALL_PREFIX` option. In the example below, we instruct CMake to install everything inside a local folder named `install` besides the `build` folder where we are launching CMake from:

```bash
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DCMAKE_INSTALL_PREFIX=../install ../GeoModel
make -j
make install
```

At the end, you will find all the libraries and executables installed in:

```bash
ls ../install
```

## How to use a custom version of Xerces-C

The Geant4 GDML format depends on teh Xerces-C library. Therefore, different Geant4 releases can use different versions of the Xerecs-C library.

If you want to build FullSimLight with a custom, locally installed Xerces-C library, you can pass the `XercesC_INCLUDE_DIR` and `XercesC_LIBRARY` variable to CMake while configuring the build of FullSimLight:

```bash
cmake  -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DXercesC_INCLUDE_DIR=<path-to-local-XercesC-installation>/include -DXercesC_LIBRARY=<path-to-local-XercesC-installation>/lib/libxerces-c.dylib ../GeoModel/
```

!!! note
    
    When setting the path of the local installation of Xerces-C, please be sure to use a full path or a local path related to the `$PWD` environment variable For example, those are valid examples:

    ```bash
    -DXercesC_INCLUDE_DIR=/opt/myXercesC/include    # using a full 'full path' 
    -DXercesC_INCLUDE_DIR=$PWD/../myXercesC/include # using a 'relative path', relative to the current build folder 
    ```

!!! note

    If you installed Geant4 on myour machine with a package manager (*e.g.*, Homebrew on macOS) the version used by the Geant4 package built by the package manager could be hardcoded in the package configuration itself. That could cause the output of warning messages by CMake, of which thev one below is an example:

    ```bash
    Value of 'XercesC_INCLUDE_DIR' is already set and does not match value set at Geant4 build-time
    ```

    Normally, those warnings do not harm the build. But please judge by yourself if the difference isn the Xerces-C library can affect the project you are working on.



## Combining different configuration/build options

You can also combine different configuration options. For example, here below we build FullsimLight by using the built-in `nlohmann-json` library together with the custom Xerces-C library, and we install the outcome in a local `../install` folder: 

```bash
cmake -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_USE_BUILTIN_JSON=1  -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DXercesC_INCLUDE_DIR=<path-to-local-XercesC-installation>/include -DXercesC_LIBRARY=<path-to-local-XercesC-installation>/lib/libxerces-c.dylib ../GeoModel/
```


