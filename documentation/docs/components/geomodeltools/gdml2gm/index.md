# GDML2GeoModel

GDML2GeoModel (gdml2gm) is a command line tool that allows to convert a GDML geometry into GeoModel format and dump it in a SQLite file (.db). At the same time *gdml2gm* library allows to plug a GDML geometry directly into *gmex* and visualize it.

*gdml2gm* has the following parameters: 

``` bash
./gdml2gm 

 **** Parameters: 
-f :   [MANDATORY] input file name [.gdml] 
-o :   [OPTIONAL]  output SQlite geometry file name [.db]

``` 

## Examples

To convert a GDML geometry and save it in SQLite format with  *gdml2gm* :
``` bash
./gdml2gm -f myGeometry.gdml -o myGeometry.db
``` 

To visualize a GDML geometry with  *gmex*, the user has to set the following environment variable: GDML_FILE_NAME. It has to point to the GDML file that the user wants to plug into *gmex*. Then it is sufficient to call gmex together with the *libGDMLtoGM.dylib/so* library:

``` bash
export GDML_FILE_NAME=<path-to-my-geometry-file>/myGeometry.gdml
./gmex ../lib/libGDMLtoGM.dylib   
``` 
