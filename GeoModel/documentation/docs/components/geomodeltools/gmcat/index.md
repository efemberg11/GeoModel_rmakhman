# GeoModelCat

GeoModelCat (`gmcat`) is a command line tool that allows to concatenate different geometry files into a single final SQLite file. It is possible to concatenate plugins (.dylib/.so) and/or SQLite (.db) files.

*gmcat* has the following parameters: 

``` bash
-o [MANDATORY] Name of the output .db file
-v [OPTIONAL] Print verbose output to the screen (default: direct verbose output to /tmp)
-g [OPTIONAL] Path to the local GeoModelATLAS repository (default: .)

``` 

## Examples

In order to combine three different plugins into one single detector file one need to call gmcat in the following way:

```bash

./gmcat libPixelGeoPlugin.0.0.1.dylib libSCTGeoPlugin.0.0.1.dylib libTRTGeoPlugin.0.0.1.dylib  -o InnerDetector.db

``` 

<!--To convert a GDML geometry and save it in SQLite format with  *gdml2gm* :-->
<!--``` bash-->
<!--./gdml2gm -f myGeometry.gdml -o myGeometry.db-->
<!--``` -->
<!---->
<!--To visualize a GDML geometry with  *gmex*, the user has to set the following environment variable: GDML_FILE_NAME. It has to point to the GDML file that the user wants to plug into *gmex*. Then it is sufficient to call gmex together with the *libGDMLtoGM.dylib/so* library:-->
<!---->
<!--``` bash-->
<!--export GDML_FILE_NAME=<path-to-my-geometry-file>/myGeometry.gdml-->
<!--./gmex ../lib/libGDMLtoGM.dylib   -->
<!--``` -->
