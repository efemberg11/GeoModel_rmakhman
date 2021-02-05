# GeoModelMassCalculator

GeoModelMassCalculator (gmmasscalculator) is a command line tool that calculates the inclusive and exclusive mass of a geometry specified as input with the -g flag. The mass report is given in an output json file (default: *gmmasscalculator_report.json*).

Run the executable with the --help option to see the available options:

``` bash
-g :   [MANDATORY] the Geometry file name [.db/.gdml/.dylib/.so] 
-o :   [OPTIONAL] mass report file name (default: gmmasscalculator_report)
``` 
By default *gmmasscalculator*, takes the main *World Volume*, and calculates the inclusive and exclusive masses of the respective daughters, saving the calculated quantities in the output json file. At the end of the report, the total masses are reported for the whole *World Volume*. The output json file format is the following:

``` bash
"exclusiveMass[kg]": 38.31620960162513,
"inclusiveMass[kg]": 208775.98847295393,
"volumeCopyNo": 16969,
"volumeEntityType": "G4Polycone",
"volumeName": "LAr::Barrel::Cryostat::MotherVolume"
``` 
where:

- *exclusiveMass* is the mass of the considered volume only (from which the volumes occupied by the daughters volumes have been subtracted)
- *inclusiveMass* is the mass of the considered volume, comprehensive of the masses of the respective daughters (propagated in an iterative way to their daughter volumes).

## Examples

To run GeoModelMassCalculator one has to specify with the -g flag the geometry file (this is mandatory). By default *gmmasscalculator* writes out the masses report in the *gmmasscalculator_report.json* file:
``` bash
./gmmasscalculator -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
``` 
To calculate the mass of a geometry described with the SQLite file *LArBarrel.db* and write out the masses report in the *mass_LArBarrel.json* file :
``` bash
./gmmasscalculator -g LArBarrel.db -o mass_LArBarrel.json 
``` 
To calculate the mass of a geometry described with one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin* and write out the masses report in the *mass_HGTD.json* file :
``` bash
./gmmasscalculator -g libHGTDPlugin.1.0.0.dylib -o mass_HGTD.json 
``` 
