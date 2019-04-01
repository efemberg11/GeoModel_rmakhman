# The 'helloGeoWrite' GeoModel example

The `helloGeoWrite` example shows you how to create a simple geometry, by using GeoModel nodes.

The example program:

 1. Builds an example geometry:
 2. Writes the geometry to an SQLite file
 3. It acess the persistent copy of the geometry, as a test


## Dependencies

### Install Qt5

To build and run GeoModel libraries, you must have a working Qt5 installation on your computer.
If you are not sure how to install it, please take a look at [the notes on Qt5, below](#notes-on-qt5).


#### GeoModelCore

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmcore
cd build_gmcore
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
cd ..
```

#### GeoModelIO

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO.git
mkdir build_gmio
cd build_gmio
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelIO
make -j 4
make install
```



## Build

From your work folder:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelExamples.git
mkdir build_hellogeo
cd build_hellogeo
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples/HelloGeoWrite/
make -j4
```

## Run

Now, you can **run the example** by typing:

```bash
./hellogeoWrite
```



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
