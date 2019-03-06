# The 'helloGeo' GeoModel example

The `helloGeo` example shows you how to read persistified GeoModel data in a standalone program.

This example program reads in GeoModel data from a `.db` file, it prints out data entries, 
it builds the GeoModel tree, it access the RootVolume, and it loops over all its child volumes, printing the name of the GeoLogVol associated with them.

It uses many of the GeoModel packages.

## Build the dependencies

### Install Qt5

To build and run GeoModel libraries, you must have a working Qt5 installation on your computer. 
If you are not sure how to install it, please take a look at [the notes on Qt5, below](notes-on-qt5).



### Build dependencies

#### Build GeoModelCore

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmcore
cd build_gmcore
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
cd ..
```
#### Build GeoModelIO dependencies

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO.git
mkdir build_gmio
cd build_gmio
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelIO
make -j 4
make install
cd ..
```



## Build 'HelloGeo'

From your work folder:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelExamples.git
mkdir build_hellogeo
cd build_hellogeo
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples/HelloGeo/
make -j4
```

Then, get sample geometry data to play with, and make a symbolic link to point to that:

```bash
wget https://atlas-vp1.web.cern.ch/atlas-vp1/doc_new/sample_datafiles/geometry/geometry-ATLAS-R2-2015-03-01-00.db
ln -s $PWD/geometry-ATLAS-R2-2015-03-01-00.db ../geometry.db
```

Now, you can **run the example** by typing:

```bash
./hellogeo
```

The example program:

 1. loads the geometry from the `.db` file
 2. prints all GeoMaterial entries found in the `.db`
 3. builds the GeoModel tree, storing it in memory
 4. gets the RootVolume of the GeoModel tree and it prints out the number of its children
 5. loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them


----

## Appendix

### Notes on Qt5

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
