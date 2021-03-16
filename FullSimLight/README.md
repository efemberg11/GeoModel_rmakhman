 FullSimLight Project
 ==========================

FullSimLight project consists of different tools based on [Geant4](https://geant4.web.cern.ch) toolkit, that can be run on multiple geometries: 

- fullSimLight: a Geant4 based light simulation (geometry, transport in magnetic field and basic physics scoring)
- gmclash: a tool that runs clash detection on your input geometry, producing a json file report
- gmgeantino: a tool to generate geantino maps from your input geometry
- gmmasscalc: tool that calculates the inclusive and exclusive mass of an input geometry
- gm2gdml: a tool to convert geometries and dump them in gdml format.

The supported geometry formats are SQLite (.db), GDML (.gdml) and dual-use plugins (.dylib/.so) as the ones described in the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins). The ATLAS detector geometry description can be imported via a SQLite or a GDML file. If the ATLAS magnetic field map is available to the user, it can be used in fullSimLight.

# Installation

## Dependencies:

FullSimLight repository has been recently merged in a big monorepository under the main [GeoModel repository](https://gitlab.cern.ch/GeoModelDev/GeoModel). 
FullSimLight project depends on [GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModel/GeoModelCore), [GeoModelIO](https://gitlab.cern.ch/GeoModelDev/GeoModel/GeoModelIO),
[Geant4](https://geant4.web.cern.ch), 
[GeoModelG4](https://gitlab.cern.ch/GeoModelDev/GeoModel/GeoModelG4) and 
[nlohmann_json](https://github.com/nlohmann/json.git)

GeoModelCore, GeoModelIO and GeoModelG4 will be built as part of the monorepository, and also nhlomann_json can be built within the monorepository.  Before installing them you need to have Geant4 installed on you machine. 
Following are the installation instructions. 


## Geant4:

Before installing Geant4, check at the Geant4 website the pre-requisites needed:
http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/gettingstarted.html
An installation of Geant4 including the GDML extension (which requires the XercesC version >=3 package installed in the system) is required, i.e. the Geant4 toolkit must be built with the -DGEANT4_USE_GDML=ON CMake option.

### Xerces-c installation
```bash
- wget https://archive.apache.org/dist/xerces/c/3/sources/xerces-c-3.2.2.tar.gz
- tar -xf xerces-c-3.2.2.tar.gz
- cd xerces-c-3.2.2 ; mkdir build  ; cd build
- cmake -DCMAKE_INSTALL_PREFIX=../../install ../
- make -j8 ; make install
```


Now that you have installed xerces-c, clone the repository at [Geant4 repo](https://gitlab.cern.ch/geant4/geant4.git), then:

```bash
git clone https://gitlab.cern.ch/geant4/geant4.git
cd geant4
git tag
```
Choose the release you want to use, i.e. Geant4-10.6.0, and checkout the corresponding tag:

```bash
git checkout tags/v10.6.0
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../  -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_BUILD_MULTITHREADED=ON
make -j8 ; make install
```
Before running ./fullSimLight and ./gmgeantino make sure to source the *geant4.sh* file to set correctly all the Geant4 environment variables. 
```bash
source <path_to_geant4_install_dir>/bin/geant4.sh
```
Alternatively, you can modifiy the .bash_profile file in the following way:
```bash
#GEANT4
export G4INSTALL=<full_path_to_Geant4_install_dir>
export G4NEUTRONHPDATA=$G4INSTALL/data/G4NDL4.6
export G4LEDATA=$G4INSTALL/data/G4EMLOW7.9.1
export G4LEVELGAMMADATA=$G4INSTALL/data/PhotonEvaporation5.5
export G4RADIOACTIVEDATA=$G4INSTALL/data/RadioactiveDecay5.4
export G4PARTICLEXSDATA=$G4INSTALL/data/G4PARTICLEXS2.1
export G4PIIDATA=$G4INSTALL/data/G4PII1.3
export G4REALSURFACEDATA=$G4INSTALL/data/RealSurface2.1.1
export G4SAIDXSDATA=$G4INSTALL/data/G4SAIDDATA2.0
export G4ABLADATA=$G4INSTALL/data/G4ABLA3.1
export G4INCLDATA=$G4INSTALL/data/G4INCL1.0
export G4ENSDFSTATEDATA=$G4INSTALL/data/G4ENSDFSTATE2.2
```

## nlohmann_json:
You can build nlohmann_json internally to the GeoModel build. Alternatively, clone the repository at [nlohmann_json repo](https://github.com/nlohmann/json.git), then:
```bash
git clone https://github.com/nlohmann/json.git
cd json
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release -DJSON_BuildTests=False ../
make -j8 ; make install
```

## FullSimLight:

Clone the new GeoModel monorepository at [GeoModel repo](https://gitlab.cern.ch/GeoModelDev/GeoModel), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../ -DGEOMODEL_BUILD_FULLSIMLIGHT=1 ../ 
make -j8 ; make install
```

If you want to build nlohman_json as part of GeoModel build, add the following cmake option:
```bash
-DGEOMODEL_USE_BUILTIN_JSON=TRUE
```

**NOTE:** If you experience issues with CMake not finding Geant4 properly, try to pass the Geant4 lib installation dir to CMake, to let it correctly find the Geant4Config.cmake file:

```bash
cmake -DGeant4_DIR=<path_to_geant4_install_dir>/lib/Geant4-10.6.0/ -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
```

Alternatively, you can source the Geant4 setup, before running cmake:

```bash
source <path_to_geant4_install_dir>/bin/geant4.sh
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
```

### How to use a custom version of Xerces-C

The Geant4 GDML format depends on the Xerces-C library. Therefore, different Geant4 releases can use different versions of the Xerces-C library. If you want to build FullSimLight with a custom, locally installed Xerces-C library, you can pass the XercesC_INCLUDE_DIR and XercesC_LIBRARY variable to CMake while configuring the build of FullSimLight:

```bash
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DXercesC_INCLUDE_DIR=<path-to-local-XercesC-installation>/include -DXercesC_LIBRARY=<path-to-local-XercesC-installation>/lib/libxerces-c.dylib ../../install
```
# Detector Construction


## Geometry:

The detector can be built starting from a SQLite .db file, from a GDML file or from a dual-use plugin like the ones in the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins). Use the -g flag to specify the name of the input geometry file. Alternatively, a GDML file can be set through the macro file by using the:

```bash
/mydet/setGdmlFile <gdml-file-name.gdml>
```    

## ATLAS Geometry Files:

The .gdml and .SQLite files of ATLAS geometry tags ATLAS-R2-2016-01-00-01 are available at: 
```bash
wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01.gdml  
wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
```
The new ATLAS geometry SQLite files where the EMEC is built using 3 new different variants (Wheel, Cone, Slices) are available at:

```bash
wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01_with_EMEC_Wheel.db
wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01_with_EMEC_Cone.db
wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01_with_EMEC_Slices.db
```

# FullSimLight: Build, run and options

The application can be built and used both with sequential and multithreaded
Geant4 builds. In case of multithreaded Geant4 toolkit, the applications will
run in proper multithreaded mode. You can find the executables under the build/bin directory and/or under the install/bin dir. 

NB: Before running fullSimLight make sure to source the *geant4.sh* file to set correctly all the Geant4 environment variables. 
```bash
source <path_to_geant4_install_dir>/bin/geant4.sh
```
Alternatively, you can modifiy the .bash_profile file in the following way:
```bash
#GEANT4
export G4INSTALL=<full_path_to_Geant4_install_dir>
export G4NEUTRONHPDATA=$G4INSTALL/data/G4NDL4.6
export G4LEDATA=$G4INSTALL/data/G4EMLOW7.9.1
export G4LEVELGAMMADATA=$G4INSTALL/data/PhotonEvaporation5.5
export G4RADIOACTIVEDATA=$G4INSTALL/data/RadioactiveDecay5.4
export G4PARTICLEXSDATA=$G4INSTALL/data/G4PARTICLEXS2.1
export G4PIIDATA=$G4INSTALL/data/G4PII1.3
export G4REALSURFACEDATA=$G4INSTALL/data/RealSurface2.1.1
export G4SAIDXSDATA=$G4INSTALL/data/G4SAIDDATA2.0
export G4ABLADATA=$G4INSTALL/data/G4ABLA3.1
export G4INCLDATA=$G4INSTALL/data/G4INCL1.0
export G4ENSDFSTATEDATA=$G4INSTALL/data/G4ENSDFSTATE2.2
```

Run the executable with the --help option to see the available options:

``` bash
-m :   REQUIRED : the standard Geant4 macro file name 
-g :   REQUIRED : the Geometry file name 
-o :   flag  ==> run the geometry overlap check (default: FALSE)
-f :   physics list name (default: FTFP_BERT) 
-P :   generate events with Pythia [config. available: ttbar/higgs/minbias or use ascii input file]
-p :   flag  ==> run the application in performance mode i.e. no user actions 
   :   -     ==> run the application in NON performance mode i.e. with user actions (default) 
``` 
FullSimLight uses by default the Geant4 particle gun as primary generator, but it supports also
input events from the Pythia generator (see the Primary generator section for more details)
A minimal set of "observable" is collected during the simulation per-primary
particle type: mean energy deposit, mean charged and neutral step lengths,
mean number of steps made by charged and neutral particles, mean number of
secondary e-, e+ and gamma particles. The result is reported at the end of
each event for each primary particle that were transported in the given event.
At the end of the simulation a final report is printed showing the run time,
the primary generator and magnetic field settings used during the run, the
total number of events and primary particles transported and the per-primary
type simulation statistics of the above-mentioned quantities.

The simulation can be executed in "performance" mode by providing the -p
input flag. No any user actions are created in this case beyond the
only one RunAction (only for the Master-thread in case of MT) that will
start and stop a timer at the beginning and the end of the simulation
(initialization time won't be included). Therefore, there is no scoring
in this case.

## Examples

During the installation a default macro file <macro.g4> will be installed in your share/FullSimLight directory.
To execute the application using the <macro.g4> macro file, with the FTFP_BERT_ATL
Physics List, in performance mode and building the detector from the geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db file :

``` bash
./fullSimLight -m ../share/FullSimLight/macro.g4 -f FTFP_BERT_ATL -p -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
``` 
To execute the application using the <macro.g4> macro file, with the FTFP_BERT_ATL
Physics List, not in performance mode and building the detector from my geometry gdml file:
``` bash
./fullSimLight -m ../share/FullSimLight/macro.g4 -f FTFP_BERT_ATL -g mygeometry.gdml
``` 
To execute the application using the <macro.g4> macro file and building the detector with a geometry described in one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin* :
``` bash
./fullSimLight -m ../share/FullSimLight/macro.g4  -g libHGTDPlugin.1.0.0.dylib
``` 

## Parameters settings via geant4 macro

Fullsimlight and in general Geant4 based simulations, need a Geant4 macro to read some input parameters. The default macro used by fullSimLight is called 'macro.g4' and it should  be found under the <install-path>/share/FullSimLight directory. The macro can be edited to change some parameters, i.e the verbosity, the number of threads, or to tune the simulation. The most relevant macro commands are explained in what follows.

## Magnetic field
 
 A constant magnetic field can be set through the macro command:
 
``` bash
     /mydet/setField <field-value> <unit>
```
The default value is a uniform magnetic field of 4 Tesla.

## ATLAS Magnetic Field

The ATLAS magnetic field is integrated in FullSimLight and can be used from it.  The Magnetic field map file used and supported are:
- full_bfieldmap_7730_20400_14m_version5.root
- solenoid_bfieldmap_7730_0_14m_version5.root
- toroid_bfieldmap_0_20400_14m_version5.root
- bmagatlas_09_fullAsym20400.data

If the magnetic field maps are not available, a constant magnetic field can be set through the macro file. The test application "testMagneticField" can be used to test the different magnetic field maps and produce Maps in 2D histograms that are saved in a ROOT file (i.e 'ATLAS_BField_default.root') .
However there is no dependency on ROOT, because the histograms are created, filled and saved using the G4AnalysisManager.

### testMagneticField examples:
Run the application with the --help flag to see the options:
``` bash
./testMagneticField --help

-f :  OPTIONAL: magnetic field filename [.data/.root]   (default : use ATLAS magnetic field maps)
-r :  FLAG:     use root field map (default : false, use .data file)
-s :  FLAG:     set Solenoid Off 
-t :  FLAG:     set Toroids Off 
```
By default the file tested is the bmagatlas_09_fullAsym20400.data. Use the -r option to test the ROOT file maps, add the -s flag to set the solenoid off and use the toroid_bfieldmap_0_20400_14m_version5.root:

``` bash
./testMagneticField -r -s 
```

Use the -t to set the Toroids off, and test the solenoid_bfieldmap_7730_0_14m_version5.root file. 

``` bash
./testMagneticField -r -t 
```


## Primary Generator

The primary generator used by default is the Geant4 particle gun, but FullSimLight also supports the  [Pythia generator](http://home.thep.lu.se/Pythia/)
 
 ## Particle gun
 The particle gun used by default  will generate primary particles
 at the (0,0,0) position with the following options:

### Number of primaries per event:
The number of primary particles per event can be set through the macro
command:

``` bash
/mygun/primaryPerEvt <number>
```

By default, i.e. if it is not specified by the above command, the number of primary particles will be randomly selected for each individual event from the [1, 10] interval uniformly.

### Primary particle energy:
The primary particle energy can be set through the macro command:

``` bash
     /mygun/energy  <energy-value> <unit>
```
By default, i.e. if it is not specified by the above command, the kinetic energy will be randomly selected for each individual primary particle from the [1 GeV, 100 GeV] uniformly.

### Primary particle direction:
The primary particle momentum direction can be set through the macro command: 

``` bash
     /mygun/direction  <xdir-value> <ydir-value> <zdir-value>
```
By default, i.e. if it is not specified by the above command, the momentum direction will be randomly selected for each individual primary particle from isotropic distribution.

### Primary particle type:
The primary particle type can be set through the macro command:

``` bash
     /mygun/particle <particle-name>
``` 
By default, i.e. if it is not specified by the above command, the type will be randomly selected from a pre-defined list for each individual primary particle uniformly. The current list of particles includes e-, e+ and gamma particles. It can be extended by adding more particles to the list in the MyPrimaryGeneratorAction class.

 ## Pythia generator
 
 FullSimLight supports Pythia as primary particles generator. In order to use Pythia, the user should have it installed in their system and if Pythia is found FullSImLight will be compiled with the support on. There are three different default options available when using the -P or --pythia flag (i.e. ttbar, higgs and minbias):
  ``` bash
 -P :   generate events with Pythia [config. available: ttbar/higgs/minbias or use ascii input file]
 ``` 
 Alternatively the user can plug their own Pythia configuration file to simulate the desired events. 
 For example, in order to simulate the default *ttbar* events, the command to be run is the following:
 
 ``` bash
./fullSimLight -m ../share/FullSimLight/pythia.g4 -P ttbar -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db 
 ``` 
 The number of events that the user wants to simulate must be specified in the g4 macro file. A specific *pythia.g4* macro file can be found in the *share* directory, that should be used when simulating Pythia events and can be edited according to the user needs. 

## Physics List

 The Physics List can be specified as an input argument with the -f flag
 (e.g. -f FTFP_BERT). Notice that the name of the Geant4 built in Physics List
 must be in upper case, exactly as the corresponding header file. By default,
 i.e. if the Physics List name is not provided as an input argument, the FTFP_BERT
 Physics List will be used. FullSimLight has also the possibility to use a
 special custom Physics List MyGVPhysicsList (a Geant4 Physics
 List including only those particles, processes and models that are available
 and used in the corresponding GeantV application with exactly the same
 settings). In order to use it one need to specify -f GV as input parameter. 

# GeoModelClash: run and options

GeoModelClash (gmclash) allows to run geometry overlap checks on a geometry file specified as input with the -g flag. It supports SQLite and GDML formats. The geometry can also be described with a dual-plugin (.dylib or .so) as the ones available at the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins). The clashes report is given in an output json file (default: gmclash_report.json).

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
* *distance* is the minimum estimated distance of the overlap
* *typeOfClash* can be 0 for *withMother*, 1 for *withSister*, 2 for *fullyEncapsSister*, 3 for *invalidSolid*
* *x,y,z* are the global coordinates of the point of impact in cm

## Examples

To run GeoModelClash one has to specify with the -g flag the geometry file (this is mandatory). By default gmclash writes out the clashes report in the *gmclash_report.json* file
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

# GeoModelGeantino: run and options

GeoModelGeantino (gmgeantino) is a Geant4 based application that allows you to produce geantino maps for the geometry specified as input. It supports .db/.gdml/.dylib/.so geometry formats and it writes out the geantino maps in a ROOT file. However, it does not depend on ROOT, cause it uses the G4AnalysisManager to create/fill/write 1D and 2D Profiles.
The 1D and 2D profiles are filled during the simulation, per Event or per Step. The creation of different profiles can be tuned with command line flags. In general X-Y, Z-R, eta-Phi RadiationLength/InteractionLength profiles can be created per DetectorVolume/Material/Element.

gmgeantino uses a default geant4 macro, called *geantino.g4*,  to take some input parameters. You should find this macro under your <install-path>/share/FullSimLight directory. You can edit this macro to change some parameters, like the verbosity, the n. of threads, the n. of primaries per event, the primary particle energy.. etc. 

By default the primary particles shot by 'gmgeantino' are geantinos (this parameter should not be changed). By default the number of simulated geantinos is set to 1000. To increase the resolution of the maps, the n. of simulated events can be increased, editing the macro and changing the value at the following line:

```bash
/run/beamOn <n. of events>
```

NB: Before running gmgeantino make sure to source the *geant4.sh* file to set correctly all the Geant4 environment variables. 
```bash
source <path_to_geant4_install_dir>/bin/geant4.sh
```
Alternatively, you can modifiy the .bash_profile file in the following way:
```bash
#GEANT4
export G4INSTALL=<full_path_to_Geant4_install_dir>
export G4NEUTRONHPDATA=$G4INSTALL/data/G4NDL4.6
export G4LEDATA=$G4INSTALL/data/G4EMLOW7.9.1
export G4LEVELGAMMADATA=$G4INSTALL/data/PhotonEvaporation5.5
export G4RADIOACTIVEDATA=$G4INSTALL/data/RadioactiveDecay5.4
export G4PARTICLEXSDATA=$G4INSTALL/data/G4PARTICLEXS2.1
export G4PIIDATA=$G4INSTALL/data/G4PII1.3
export G4REALSURFACEDATA=$G4INSTALL/data/RealSurface2.1.1
export G4SAIDXSDATA=$G4INSTALL/data/G4SAIDDATA2.0
export G4ABLADATA=$G4INSTALL/data/G4ABLA3.1
export G4INCLDATA=$G4INSTALL/data/G4INCL1.0
export G4ENSDFSTATEDATA=$G4INSTALL/data/G4ENSDFSTATE2.2
```

Run the executable with the --help option to see the available options:

``` bash
./gmgeantino --help

-g :   [REQUIRED] the Geometry file name (supported extensions: .db/.gdml/.dylib/.so) 
-m :   [OPTIONAL] the standard Geant4 macro file name (default: 'geantino.g4') 
-r :   [OPTIONAL] r limit for geantino maps in mm (default: '12500') 
-z :   [OPTIONAL] z limit for geantino maps in mm (default: '23000') 
-x :   [OPTIONAL] x limit for geantino maps in mm (default: '12500') 
-y :   [OPTIONAL] y limit for geantino maps in mm (default: '12500') 
-o :   [OPTIONAL] output ROOT file name  (supported extention: .root - default: 'geantinoMaps.root') 
-e :   [FLAG]     use this flag to create eta-phi radiation-interaction length 1D profile histograms (caveat: the process might run out of memory!)
-d :   [FLAG]     use this flag to create xy-rz   radiation-interaction length 2D profile histograms for 'detectors' (caveat: the process might run out of memory!)
-a :   [FLAG]     use this flag to create xy-rz   radiation-interaction length 2D profile histograms for 'materials' (caveat: the process might run out of memory!)
-l :   [FLAG]     use this flag to create xy-rz   radiation-interaction length 2D profile histograms for 'elements'  (caveat: the process might run out of memory!)
``` 


## Examples

To run *gmgeantino* one has to specify with the -g flag the input geometry file (mandatory option). By default a 'geantinoMaps.root' file is created as output and it contains R-Z RadiationLenght and Interaction Lenght 2D profiles. To run gmgeantino on *LArBarrel.db* geometry, with the default *geantino.g4* macro file, and producing eta-phi maps and detector maps:

``` bash
./gmgeantino -m ../share/FullSimLight/geantino.g4 -g LArBarrel.db -e -d 
``` 

To produce geantino maps of a geometry described by one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin*, using a custom macro file *mymacro.g4*, activate detectors/materials and elements maps, and write out the geantino maps in the *geantinoMaps_HGTD.root* file :
``` bash
./gmgeantino -m mymacro.g4 -g libHGTDPlugin.1.0.0.dylib -d -a -l -o geantinoMaps_HGTD.root 
``` 

# GeoModelMassCalculator: run and options

# GeoModelMassCalculator

GeoModelMassCalculator (gmmasscalc) is a command line tool that calculates the inclusive and exclusive mass of a geometry specified as input with the -g flag. The mass report is given in an output json file (default: *gmmasscalc_report.json*).

Run the executable with the --help option to see the available options:

``` bash
-g :   [MANDATORY] the Geometry file name [.db/.gdml/.dylib/.so] 
-p :   [OPTIONAL] prefix of the Logical Volumes of interest (i.e. Pixel::) 
-m :   [OPTIONAL] material of interest (i.e. Aluminium) 
-o :   [OPTIONAL] mass report json file name (default: gmmasscalc_report.json)
-v :   [OPTIONAL] verbose mode: 1 - print all the Logical Volume names and materials, 2 - print materials composition (default: off)
``` 
By default (if the optional flag are not used) *gmmasscalc*, takes the main *World Volume*, and calculates the inclusive and exclusive masses of the respective daughters, saving the calculated quantities in the output json file. At the end of the report, the total masses are reported for the whole *World Volume*. The output json file format is the following:

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

At the end of the report, additional information about the whole geometry is reported. The last item of the json file will look like the following:

``` bash
"apparentWeightInAir[kg]": 2405.2018407511378,
"densityThreshold[g/cm3]": 0.02,
"excludedFilteredMass[kg]": 14.649883998898723,
"exclusiveMass[kg]": 1.3704943478464804,
"inclusiveFilteredMass[kg]": 2406.500411630485,
"inclusiveMass[kg]": 2421.156760687377,
"logicalVolumeName": "newWorldLog",
"material": "Air",
"volumeEntityType": "World Volume"
``` 
where:
- *apparent weight in Air* by definition, the weight of a body as affected by the buoyancy of a fluid (such as air) in which it is immersed. It is calculated only on the total geometry, assuming that the World Volume is made of Air
- *exclusiveFilteredMass* is the sum of the exclusive masses of all the volumes with density>densityThreshold (0.02 g/cm3)
-*excludedFilteredMass*  is the sum of the exclusive masses of all the volumes with density<densityThreshold (0.02 g/cm3)

In addition to the default behaviour, *gmmasscalc* offers the possibility to apply 2 filters to the geometry, described in what follows. 

The -p (--prefix) option allows to indicate the prefix of the volumes of interest. In this case  *gmmasscalc* will loop over the geometry tree and calculate the mass of every Logical Volume that has the specified prefix in its name. For every Logical volume found, a different entry will be filled in the output report file.

The -m (--material) option allows to specify to which material the user is interested. In this case  *gmmasscalc* will loop over the geometry tree and calculate the mass of those volumes that are made of the material of interest. 

If both the -p and -m flags are used, *gmmasscalc* will combine the 2 filters and retrieve in the output file only the masses of the volumes containing the specified prefix and made of the desired material. 

At the end of the report, the total exclusive mass for the requested geometry is  reported for all the volumes that satisfy the user request. The last item in the report will look like the following:

```bash
{
"exclusiveMass[kg]": 14.676774220119631,
"inclusiveMass[kg]": -999.0,
"logicalVolumeName": "",
"material": "CO2",
"physicalVolumeName": "",
"volumeCopyNo": 0,
"volumeEntityType": "Total for filtered Geometry"
}

```
where:
- *logicalVolumeName*  and *material* are respectively the prefix used as filter for the Logical Volume names in the geometry tree navigation, and the material used as a second filter in the search. 

The -v flag is a verbosity flag. Two different levels of verbosity can be set: level 1 prints all the logical volume names and respective materials, level 2 adds also the material composition for each material in the geometry. 

## Examples

To run GeoModelMassCalculator one has to specify with the -g flag the geometry file (this is mandatory). By default *gmmasscalc* writes out the masses report in the *gmmasscalc_report.json* file:
``` bash
./gmmasscalc -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
``` 
To calculate the mass of a geometry described with the SQLite file *LArBarrel.db* and write out the masses report in the *mass_LArBarrel.json* file :
``` bash
./gmmasscalc -g LArBarrel.db -o mass_LArBarrel.json 
``` 
To calculate the mass of a geometry described with the SQLite file *SCT.db*, only for the logical volumes that have *BRLSensor* in their names, and only for those volumes that are made of *Silicon* and write out the masses report in the *mass_SCT.json* file:

``` bash
./gmmasscalc -g SCT.db -p BRLSensor -m Silicon -o mass_SCT.json 
``` 
To calculate the mass of a geometry described with one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin*, looking for volumes made of *Aluminium* and write out the masses report in the *mass_HGTD.json* file :
``` bash
./gmmasscalc -g libHGTDPlugin.1.0.0.dylib -m Aluminium -o mass_HGTD.json 
``` 

# GeoModel2GDML: run and options

GeoModel2GDML (gm2gdml) is a command line tool based on Geant4, to convert geometries and dump them in gdml.
The geometry is first built in GeoModel, converted in Geant4 and dumped in gdml format.
The supported input geometries are : sqlite (.db), plugin (.so/dylib) and gdml.
Before the dump operation the geometry is revised and the unidentified volumes are pulled out from the geometry.
Run the executable with the --help option to see the available options:

``` bash
./gm2gdml --help

 **** Parameters: 
-g :   [MANDATORY] input  geometry file name [.db/.gdml/.dylib/.so] 
-o :   [OPTIONAL]  output GDML geometry file name (default: geometry.gdml)

``` 


## Examples

To run GeoModel2GDML one has to specify with the -g flag the geometry file (this is mandatory). By default a 'geometry.gdml' output file is created, but the name of the file can be changed using the -o option. 
To run gm2gdml on *LArBarrel.db* geometry, and dump the output gdml file in the *LArBarrel.gdml* file:
``` bash
./gm2gdml -g LArBarrel.db -o LArBarrel.gdml
``` 

To dump your geometry described with a plugin, i.e.  *HGTDPlugin*, and write out the geometry file in the *HGTDPlugin.gdml* file :
``` bash
./gm2gdml -g libHGTDPlugin.1.0.0.dylib -o HGTDPlugin.gdml
``` 


# Final Remark

 To get the most performant Geant4 build, one needs to build the Geant4 toolkit
 with the following CMake options (in addition to -DGEANT4_USE_GDML=ON):
 
``` bash
 -DGEANT4_BUILD_MULTITHREADED=ON
 -DGEANT4_BUILD_VERBOSE_CODE=OFF
 -DGEANT4_BUILD_STORE_TRAJECTORY=OFF
``` 
 Then set the number of threads in the macro.g4 macro to the maximum available
and execute the fullSimLight application with the -p flag.
