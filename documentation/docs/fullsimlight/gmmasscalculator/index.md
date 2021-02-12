# GeoModelMassCalculator

GeoModelMassCalculator (gmmasscalculator) is a command line tool that calculates the inclusive and exclusive mass of a geometry specified as input with the -g flag. The mass report is given in an output json file (default: *gmmasscalculator_report.json*).

Run the executable with the --help option to see the available options:

``` bash
-g :   [MANDATORY] the Geometry file name [.db/.gdml/.dylib/.so] 
-p :   [OPTIONAL] prefix of the Logical Volumes of interest (i.e. Pixel::) 
-m :   [OPTIONAL] material of interest (i.e. Aluminium) 
-o :   [OPTIONAL] mass report json file name (default: gmmasscalculator_report.json)
``` 
By default (if the optional flag are not used) *gmmasscalculator*, takes the main *World Volume*, and calculates the inclusive and exclusive masses of the respective daughters, saving the calculated quantities in the output json file. At the end of the report, the total masses are reported for the whole *World Volume*. The output json file format is the following:

``` bash
"exclusiveMass[kg]": 1.3358800280858636,
"inclusiveMass[kg]": 133.81273262584054,
"logicalVolumeName": "SCT_ForwardC",
"material": "Air",
"physicalVolumeName": "SCT",
"volumeCopyNo": 16969,
"volumeEntityType": "G4Tubs"
``` 
where:

- *exclusiveMass* is the mass of the considered volume only (from which the volumes occupied by the daughters volumes have been subtracted)
- *inclusiveMass* is the mass of the considered volume, comprehensive of the masses of the respective daughters (propagated in an iterative way to their daughter volumes).

The -p (--prefix) option allows to indicate the prefix of the volumes of interest. In this case  *gmmasscalculator* will loop over the geometry tree and calculate the mass of every Logical Volume that has the specified prefix in its name. For every Logical volume found, a different entry will be filled in the output report file.

The -m (--material) option allows to specify to which material the user is interested. In this case  *gmmasscalculator* will loop over the geometry tree and calculate the mass of those volumes that are made of the material of interest. 

If both the -p and -m flags are used, *gmmasscalculator* will combine the 2 filters and retrieve in the output file only the masses of the volumes containing the specified prefix and made of the desired material. 

In any case, at the end of the report, the total inclusive and exclusive masses are reported for all the volumes that satisfy the user request. The last item in the report will look like the following:

```bash
{
"exclusiveMass[kg]": 22.8290395217687,
"inclusiveMass[kg]": 22.8290395217687,
"logicalVolumeName": "BRLSensor",
"material": "Silicon",
"physicalVolumeName": "",
"volumeCopyNo": 0,
"volumeEntityType": "Total for requested Geometry"
}

```
where:

- *logicalVolumeName*  and *material* are respectively the prefix used as filter for the Logical Volume names in the geometry tree navigation, and the material used as a second filter in the search. 


## Examples

To run GeoModelMassCalculator one has to specify with the -g flag the geometry file (this is mandatory). By default *gmmasscalculator* writes out the masses report in the *gmmasscalculator_report.json* file:
``` bash
./gmmasscalculator -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
``` 
To calculate the mass of a geometry described with the SQLite file *LArBarrel.db* and write out the masses report in the *mass_LArBarrel.json* file :
``` bash
./gmmasscalculator -g LArBarrel.db -o mass_LArBarrel.json 
``` 
To calculate the mass of a geometry described with the SQLite file *SCT.db*, only for the logical volumes that have *BRLSensor* in their names, and only for those volumes that are made of *Silicon* and write out the masses report in the *mass_SCT.json* file:

``` bash
./gmmasscalculator -g SCT.db -p BRLSensor -m Silicon -o mass_SCT.json 
``` 
To calculate the mass of a geometry described with one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin*, looking for volumes made of *Aluminium* and write out the masses report in the *mass_HGTD.json* file :
``` bash
./gmmasscalculator -g libHGTDPlugin.1.0.0.dylib -m Aluminium -o mass_HGTD.json 
``` 
