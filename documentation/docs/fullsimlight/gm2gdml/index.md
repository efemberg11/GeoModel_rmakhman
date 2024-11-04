# GeoModel2GDML

GeoModel2GDML (`gm2gdml`) is a command line tool based on Geant4, to convert geometries and dump them in gdml.
The geometry is first built in `GeoModel`, converted in `Geant4` and dumped in gdml format.
The supported input geometries are : SQlite (.db), plugin (.so/dylib) and gdml.
Before the dump operation the geometry is revised and the unidentified volumes are pulled out from the geometry.
Run the executable with the `--help` option to see the available options:

``` bash
./gm2gdml --help

 **** Parameters: 
-g :   [MANDATORY] input  geometry file name [.db/.gdml/.dylib/.so] 
-o :   [OPTIONAL]  output GDML geometry file name (default: geometry.gdml)

``` 


## Examples

To run `gm2gdml` one has to specify with the `-g` flag the geometry file (mandatory option). By default a `geometry.gdml` output file is created, but the name of the file can be changed using the `-o` option. 
To run `gm2gdml` on `LArBarrel.db` geometry, and dump the output gdml file in the `LArBarrel.gdml` file:
``` bash
./gm2gdml -g LArBarrel.db -o LArBarrel.gdml
``` 

To dump your geometry described with a plugin, i.e.  `HGTDPlugin`, and write out the geometry file in the `HGTDPlugin.gdml` file :
``` bash
./gm2gdml -g libHGTDPlugin.1.0.0.dylib -o HGTDPlugin.gdml
``` 
