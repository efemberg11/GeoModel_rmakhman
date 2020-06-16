
# Notes on Qt5

To build and run the GeoModelIO libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at the notes on Qt5, here below.


Also, you can take a look at the official documentation at: <https://doc.qt.io/qt-5/gettingstarted.html>

## Ubuntu 18

On Ubuntu 18, you can use the default Qt5 installation shipped with the operating system.

## macOS

On macOS, you can install Qt5 by using `brew`:

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

## Centos7 / CC7

*coming*

## SLC6

*coming*

## LXPLUS (CERN only)

*coming*
