# GeoModelClash

GeoModelClash (gmclash) allows to run geometry overlap checks on a geometry file specified as input with the -g flag. The clashes report is given in an output json file (default: *gmclash_report.json*).

Run the executable with the --help option to see the available options:

``` bash
-g :   MANDATORY: the Geometry file name [.db/.gdml/.dylib/.so] 
-o :   OPTIONAL : clashes report file name (default: gmclash_report.json)
``` 
The output json file format is the following:

``` bash
"distance[mm]": 51.21328179620343,
"typeOfClash": 1,
"volume1CopyNo": 16969,
"volume1EntityType": "G4Tubs",
"volume1Name": "LAr::Barrel::Cryostat::Cylinder::#13Phys",
"volume2CopyNo": 16969,
"volume2EntityType": "G4UnionSolid",
"volume2Name": "LAr::DM::SectorEnvelopes2r",
"x": -1.736718203796568,
"y": -1263.348806272393,
"z": -166.75403155804725
``` 
where:

-  *distance* is the minimum estimated distance of the overlap
- *typeOfClash* can be 0 for *withMother*, 1 for *withSister*, 2 for *fullyEncapsSister*, 3 for *invalidSolid*
-  *x,y,z* are the global coordinates of the point of impact in cm

N.B. A solid is considered invalid, when it is the results of boolean operations that are "not safe"or wrong (i.e. summing up two solids that do not share any portion of their volumes). Usually an invalid solid is a solid for which it is not possible to correctly generate points on the surface. One example can be of a *solidC* that is the result of the subtraction of *solidA* - *solidB*, where *solidB* is bigger than *solidA*. This operation leads to a  *NULL* volume that is considered as an invalid solid by *gmclash*.

## Examples

To run GeoModelClash one has to specify with the -g flag the geometry file (this is mandatory). By default gmclash writes out the clashes report in the *gmclash_report.json* file:
``` bash
./gmclash -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
``` 
To execute a clash detection on a geometry described with the SQLite file *LArBarrel.db* and write out the clashes report in the *cr_LArBarrel.json* file :
``` bash
./gmclash -g LArBarrel.db -o cr_LArBarrel.json 
``` 
To execute a clash detection on a geometry described with one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin* and write out the clashes report in the *cr_HGTD.json* file :
``` bash
./gmclash -g libHGTDPlugin.1.0.0.dylib -o cr_HGTD.json 
``` 
## How to visualize the gmclash clashes report in gmex

Once the json clashes report has been produced, it can be plugged into *gmex* together with the geometry file that it's been inspected and visualized. In order to do that, it is sufficient to open the inspected geometry with *gmex* and then click on the *Clash Points* tab, push the *Open* button, and select the corresponding clash report file, as it can be seen in the Figure 1. 

{{ imgutils_image_caption('gmclash_1.png', 
   alt='ClashPoints in gmex', 
   width=600,
   cap='Figure 1: How to load the clash points report file in gmex.',
   urlFix=False) 
}}

Once the clash report file has been opened, the clash points will be displayed in different colours (for the different types of clashes) together with the inspected geometry, like it can be seen in Figure 2. 

{{ imgutils_image_caption('gmclash_2.png', 
   alt='cp', 
   width=600,
   cap='Figure 2 : Clash points detected with gmclash and visualized in gmex.',
   urlFix=False) 
}}
