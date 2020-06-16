# GeoGenericFunctions

A subset of QatGenericFunctions (see <qat.pitt.edu>) needed for GeoModelKernel.

## Compilation

### Simple compilation

To compile it interactively, for development, just do the usual:

```bash
mkdir build
cd build
cmake ../GeoGenericFunctions/
make
make install
```

### Compilation for Athena-code development

Usually, Externals packages are buit and installed through the build of the package [atlasexternals](), downloading and compiling the sources of the different packages and installing them in an "install" folder, to let Athena-code uses them through a series of scripts.
However, this implies that, if you change the source code of an External package, you must push your changes somewhere (or you must package them ina zip file) for the `atlasexternals` build script to get your latest changes. And that implies a series of additional steps, which are not convenient if you are actively debugging or developing code.

Thus, after having installed the `atlasexternals` as usual (let's assume, using the path `../../install/VP1LightExternals/` as `DISTDIR`; see the [atlasexternals docs](https://gitlab.cern.ch/atlas/atlasexternals/blob/master/README.md) for further details, or [this guide for developers](https://gitlab.cern.ch/atlas/atlasexternals/blob/master/CONTRIBUTING.md)), if you want to compile interacively this external while you are developing Athena or AnalysisBase/VP1Light packages, you can use these example commands (replace the paths with your own):


```bash
cmake -DCMAKE_INSTALL_LIBDIR=../../install/VP1LightExternals/InstallArea/x86_64-slc6-gcc62-opt -DCMAKE_INSTALL_PREFIX=../../install/VP1LightExternals/InstallArea/x86_64-slc6-gcc62-opt ../GeoGenericFunctions/
make
make install
```

In this way, `make install` will install both the headers and the shared library within the path used by the default Athena setup.

With this method, you can change GeoGenericFunction's code in one of your shells, compile and install that, and use the new changes in the Athena-based code you are developing, which searches for shared libraries inside, for example, the `../../install/VP1LightExternals` folder (see the [**atlasexternals** documentation](https://gitlab.cern.ch/atlas/atlasexternals) for further details).
