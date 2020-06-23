# The 'HelloToyDetectorFactory' GeoModel example

![](docs/assets/visualization-e7c35926.png)

The `HelloToyDetectorFactory` example shows you how to create a complex geometry using the GeoModel DetectorFactory and Scrutinizers.

The example program:

 1. Builds an example geometry, by using a DetectorFactory
 2. Accesses the in-memory geometry:
   * gets the RootVolume of the GeoModel tree and it prints out the number of its children
   * loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them
 3. Write the geometry to a local file


## Dependencies

### Install Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer.
If you are not sure how to install it, please take a look at the Appendix, below.


## Quick Instructions

### macOS Mojave

To quickly install everything, we use [Homebrew](). If you don't have installed it on your computer, please install it by following the [instructions on its website]().

Install the dependencies, [GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModelCore) and [GeoModelIO](https://gitlab.cern.ch/GeoModelDev/GeoModelIO):

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install geomodelcore geomodelio
```

After that, get the example code and compile the example:

```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelExamples.git
mkdir build_hellotoydetectorfactory
cd build_hellotoydetectorfactory
cmake ../GeoModelExamples/HelloToyDetectorFactory
make
```

Then you can run the example program:

```
./helloToyDetectorFactory
```

When the program terminates, you will find a `geometry.db` file in the build folder. This is a SQLite file containing the dump of the geometry defined in the example C++ code.

## Visualization

You can visualize it by opening it with VP1Light. Please refer to the [Visualization documentation](docs/visualization.md).


----

## Appendix

### Notes on Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at [the notes on Qt5, in the GeoModelIO repository](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/blob/master/README_QT5_NOTES.md).
