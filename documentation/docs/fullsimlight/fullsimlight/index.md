# FullSimLight

FullSimLight package consists of different tools based on [Geant4](https://geant4.web.cern.ch) toolkit, that can be run on multiple geometries: 

- fullSimLight: a light particle transport simulation (geometry, transport in magnetic field and basic physics scoring)
- gmclash: a tool that runs clash detection on your input geometry, producing a json file report
- gmgeantino: a tool to generate geantino maps from your input geometry
- gm2gdml: a tool to convert geometries and dump them in gdml format.

## Detector Construction

### Geometry:

The supported geometry formats are SQLite (.db), GDML (.gdml) and plugins that build from scratch a GeoModel geometry (.dylib/.so). An example of geometry plugins can be found in the [GeoModelExamples folder](https://gitlab.cern.ch/GeoModelDev/GeoModel/-/tree/master/GeoModelExamples/KitchenSinkPlugin). 
Use the -g flag to specify the name of the input geometry file. 

### ATLAS Geometry Files:

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
## FullSimLight: run and options

The fullSimLight application can be built and used both with sequential and multithreaded Geant4 builds. In case of multithreaded Geant4 toolkit, the applications will run in proper multithreaded mode. You can find the executables under the build/bin directory and/or under the *<  path-to-install > /bin* dir. 

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
-P :   use Pythia primary generator [config. available: ttbar/higgs/minbias or use a Pythia command input file]
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

During the installation a default macro file *<  macro.g4 >* will be installed in your *< install-path >/share/FullSimLight* directory.

To execute the application using the < macro.g4 >  macro file, with the default FTFP_BERT
Physics List, not in performance mode and building the detector from < mygeometry.gdml > file:

``` bash
./fullSimLight -m ../share/FullSimLight/macro.g4 -g mygeometry.gdml
``` 

To execute the application using the <  macro.g4 >  macro file and building the detector with a geometry described in one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *HGTDPlugin* :

``` bash
./fullSimLight -m ../share/FullSimLight/macro.g4  -g libHGTDPlugin.1.0.0.dylib
``` 

To execute the application using the <  macro.g4 >  macro file, with the customized ATLAS FTFP_BERT_ATL Physics List, in performance mode and building the detector from the geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db file :

``` bash
./fullSimLight -m ../share/FullSimLight/macro.g4 -f FTFP_BERT_ATL -p -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db
``` 

N.B. In order to have the FTFP_BERT_ATL available in Geant4, you need to install an ATLAS patched version of Geant4, that you can find [here](https://gitlab.cern.ch/atlas-simulation-team/geant4/-/tags).

## Parameters settings via Geant4 macro

FullSimLight and in general Geant4 based simulations, need a Geant4 macro to read some input parameters. The default macro used by fullSimLight is called 'macro.g4' and it should  be found under the *<  install-path > /share/FullSimLight* directory. The macro can be edited to change some parameters, i.e the verbosity, the number of threads, or to tune the simulation. The most relevant macro commands are explained in what follows.

## Magnetic field
 
 A constant magnetic field can be set through the macro command:
 
``` bash
/mydet/setField <field-value> <unit>
```

The default value is a uniform magnetic field of 4 Tesla.

## ATLAS Magnetic Field

The ATLAS magnetic field is integrated in FullSimLight and can be used from it.  The magnetic field map file used and supported are:

- full_bfieldmap_7730_20400_14m_version5.root
- solenoid_bfieldmap_7730_0_14m_version5.root
- toroid_bfieldmap_0_20400_14m_version5.root
- bmagatlas_09_fullAsym20400.data

If the magnetic field maps are not available to the user, a constant magnetic field can be set through the macro file. The test application "testMagneticField" can be used to test the different magnetic field maps and produce Maps in 2D histograms that are saved in a ROOT file (i.e 'ATLAS_BField_default.root') .
Please notice that no dependency on ROOT is introduced, because the histograms are created, filled and saved using the *G4AnalysisManager*.

### testMagneticField examples:
Run the application with the --help flag to see the options:

``` bash
./testMagneticField --help

-f :  OPTIONAL: magnetic field filename [.data/.root]   (default : use ATLAS magnetic field maps)
-r :  FLAG:     use root field map (default : false, use .data file)
-s :  FLAG:     set Solenoid Off 
-t :  FLAG:     set Toroids Off 
```

By default the file tested is the *bmagatlas_09_fullAsym20400.data*. Use the -r option to test the ROOT file maps, add the -s flag to set the solenoid off and use the *toroid_bfieldmap_0_20400_14m_version5.root*:

``` bash
./testMagneticField -r -s 
```

Use the -t to set the Toroids off, and test the *solenoid_bfieldmap_7730_0_14m_version5.root* file. 

``` bash
./testMagneticField -r -t 
```

## Primary Generators

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
 
 FullSimLight supports Pythia as primary particles generator. In order to use Pythia, the user should have it installed in their system and if Pythia is found FullSimLight will be compiled with the support on. There are three different default options available when using the -P or --pythia flag (i.e. *ttbar*, *higgs* and *minbias*):
 
  ``` bash
 -P :   generate events with Pythia [config. available: ttbar/higgs/minbias or use ascii input file]
 ``` 
 
 Alternatively the user can plug their own Pythia configuration file to simulate the desired events. 
 For example, in order to simulate the default *ttbar* events, the command to be run is the following:
 
 ``` bash
./fullSimLight -m ../share/FullSimLight/pythia.g4 -P ttbar -g geometry-ATLAS-R2-2016-01-00-01_wSPECIALSHAPE.db 
 ``` 
 
 The number of events that the user wants to simulate must be specified in the Geant4 macro file. A specific *pythia.g4* macro file can be found in the *<  path-to-install >/share/FullSimLight* directory, that should be used when simulating Pythia events and can be edited according to the user needs. 

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
