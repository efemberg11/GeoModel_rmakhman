# The 'GeoShapeExamples' GeoModel example

![](docs/assets/visualization-3318dc3d.png)

The `GeoShapeExamples` example shows you how to use different GeoShape nodes (GeoBox, GeoTorus, GeoTessellatedSolid, ...) and saves them to a local file.

The example program:

 1. Builds different example GeoModel shapes
 2. Writes the outcome geometry to an SQLite file


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
mkdir build_geoshapes
cd build_geoshapes
cmake ../GeoModelExamples/GeoShapeExamples
make
```

Then you can run the example program:

```
./geoShapeExamples
```

When the program terminates, you will find a `geometry.db` file in the build folder. This is a SQLite file containing the dump of the geometry defined in the example C++ code.

## Visualization

You can visualize it by opening it with VP1Light. Please refer to the [Visualization documentation](docs/visualization.md).


----

## Dependencies

### Install Qt5

To build and run GeoModel libraries, you must have a working Qt5 installation on your computer.
If you are not sure how to install it, please take a look at [the notes on Qt5, below](#notes-on-qt5).


----

## Appendix

### Notes on Qt5

To build and run GeoModel libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at [the notes on Qt5, below](#notes-on-qt5).

See: <https://doc.qt.io/qt-5/gettingstarted.html>

**On Ubuntu 18**, you can use the default Qt5 installation shipped with Ubuntu.

**On macOS**, you can install Qt5 by using `brew`:

```bash
brew install qt5
```

After the installation, pay attention to add the Qt folder to your PATH (replace <path-to-qt> with your Qt installation folder and 'clang_64' with ):

```bash
PATHQT=/<path-to-qt>/Qt5.12.0/5.12.0/clang_64;
export PATH=$PATHQT/bin:$PATH;
```

On some platforms, you might need to explicitly set the QMAKESPEC and the QT variables:

```bash
export QMAKESPEC=$PATHQT/mkspecs/macx-clang;
export QTDIR=$PATHQT;
export QTINC=$PATHQT/include;
export QTLIB=$PATHQT/lib
```
