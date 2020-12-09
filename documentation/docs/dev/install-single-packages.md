
# Install single GeoModel packages to speed up your development cycle

Most of the times, you need to install all the base GeoModel libraries and tools; for example, when working on the development of sub-detectors' plugins. And for that you can refer to the [Install page](../start/install.md).

Sometimes, however, you want to develop some of the GeoModel packages, but you don't want to build the whole software stack from sources every time. 

For that use case, you can install the insgle binary packages distributed through the GeoModel distribution kits. 


## macOS

You might have noticed, in the [Install]() page, that installing the package `geomodelvisualization` also installs all the base libraries and their dependencies. However, you can install single packages too.

### Install the GeoModel base libraries

```
brew install geomodel
```

This installs only the base packages:  `GeoModelCore` and `GeoModellIO`.

This is useful when you want to develop the `GeoModelTools` or `GeoModelVisualization` packages only, and you don't want to take care of the build of the base libraries too.



### Install the graphical libraries

```
brew install soqt-geomodel
```

This installs the graphical libraries needed for the 3D graphics: `simage`, `coin`, and `soqt`. Installing `soqt-geomodel` will install all the three libraries. But you can install them separately, if you need:

```
brew install simage-geomodel
brew install coin-geomodel
brew install soqt-geomodel
```

This is especially useful when testing new versions of the graphical libraries.


### All Homebrew formulas

You can take a look at all available formulas at the [packaging repository](https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel).

!!! warning

    Please note, that not all formulas in the packaging repository are meant for end use. Some of them are for testing only, some are meant to be used by package managers only, some are experimental, and some are outdated. So, please be careful when trying formulas that are not explicitely listed in this page.



