 FullSimLight Project
 ==========================

 In this directory you can find the FullSimLight project,
 a [Geant4](https://geant4.web.cern.ch) based simulation of the full 
 ATLAS detector.
 
 The ATLAS geometry can be imported via a .db or a .gdml file.

 An installation of Geant4 including the GDML extension (which requires
 the XercesC package installed in the system) is required i.e. the
 Geant4 toolkit must be built with the -DGEANT4_USE_GDML=ON CMake option.


# Installation

## Dependencies:

FullSimLight project depends on [GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModelCore), [GeoModelIO](https://gitlab.cern.ch/GeoModelDev/GeoModelIO),
[Geant4](https://geant4.web.cern.ch) and 
[GeoModelG4](https://gitlab.cern.ch/GeoModelDev/GeoModelG4)
Follow the instructions at the respective repositories to install them. 

## GeoModelCore:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/GeoModelCore), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
cd GeoModelCore
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```
## GeoModelIO:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/GeoModelIO), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
cd GeoModelIO
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```

## Geant4:

Before installing Geant4, check at the Geant4 website the pre-requisites needed:
http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/gettingstarted.html
Note in particular that to enable the use of geometry reading/writing from GDML XML files, the Xerces-C++ headers and library >=3 must be installed.
Clone the repository at this [address](https://gitlab.cern.ch/geant4/geant4.git), then:

```bash
git clone https://gitlab.cern.ch/geant4/geant4.git
cd geant4
git tag
```
Choose the release you want to use, i.e. Geant4-10.6.0, and checkout the corresponding tag:

```bash
git checkout tags/v10.6.0
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../  -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -GEANT4_BUILD_MULTITHREADED=ON
make
make install
```

## GeoModelG4:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/GeoModelG4), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelG4.git
cd GeoModelG4
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```

## FullSimLight:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/FullSimLight), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/FullSimLight.git
cd FullSimLight
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```

If you experience issues with CMake not finding GeoModelPackages properly, try to pass their lib/cmake installation dir to CMake, to let it correctly find the xxxConfig.cmake file:

```bash
cmake -DGeoModelCore_DIR=<path_to_geomodelcore>/lib/cmake/GeoModelCore/ -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
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


 # Detector


## Detector construction:

The detector can be built starting from a SQLite .db file (default option) or starting from a GDML file.
the -d flag will be used to specify the name of the geometry file, while the -g flag must be specified if you want to build the detector from a GDML file. Any GDML file can be set from the macro file by using the:

```bash
     /mydet/setGdmlFile <gdml-file-name.gdml>
```    
The default values for the SQLite and GDML files are respectively
"ATLAS-R2-2016-01-00-01.db" and "ATLAS-R2-2016-01-00-01.gdml".
     
## ATLAS Geometry Files:
     
The .gdml and .SQLite files of ATLAS geometry tags ATLAS-R2-2016-01-00-01 are available at: 
```bash
   wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01.gdml  
   wget https://gitlab.cern.ch/GeoModelATLAS/geometry-data/raw/master/geometry/geometry-ATLAS-R2-2016-01-00-01.db
```


 # Magnetic field
 
 A constant magnetic field can be set through the macro command:
 
``` bash
     /mydet/setField <field-value> <unit>
```
The default value is zero i.e. no magnetic field is created.


 # Primary Generator

 The primary generator is a particle gun that will generate primary particles
 at the (0,0,0) position with the following options:

## Number of primaries per event:
The number of primary particles per event can be set through the macro
command:

``` bash
/mygun/primaryPerEvt <number>
```

By default, i.e. if it is not specified by the above command, the number of primary particles will be randomly selected for each individual event from the [1, 10] interval uniformly.

## Primary particle energy:
The primary particle energy can be set through the macro command:

``` bash
     /mygun/energy  <energy-value> <unit>
```
By default, i.e. if it is not specified by the above command, the kinetic energy will be randomly selected for each individual primary particle from the [1 GeV, 100 GeV] uniformly.

## Primary particle direction:
The primary particle momentum direction can be set through the macro command: 

``` bash
     /mygun/direction  <xdir-value> <ydir-value> <zdir-value>
```
By default, i.e. if it is not specified by the above command, the momentum direction will be randomly selected for each individual primary particle from isotropic distribution.

## Primary particle type:
The primary particle type can be set through the macro command:

``` bash
     /mygun/particle <particle-name>
``` 
By default, i.e. if it is not specified by the above command, the type will be randomly selected from a pre-defined list for each individual primary particle uniformly. The current list of particles includes e-, e+ and gamma particles. It can be extended by adding more particles to the list in the MyPrimaryGeneratorAction class.



# Physics List

 The Physics List can be specified as an input argument with the -f flag
 (e.g. -f FTFP_BERT). Notice that the name of the Geant4 built in Physics List
 must be in upper case, exactly as the corresponding header file. By default,
 i.e. if the Physics List name is not provided as an input argument, the FTFP_BERT
 Physics List will be used. FullSimLight has also the possibility to use a
 special custom Physics List MyGVPhysicsList (a Geant4 Physics
 List including only those particles, processes and models that are available
 and used in the corresponding GeantV application with exactly the same
 settings). In order to use it one need to specify -f GV as input parameter. 



# Build, run and options

 The application can be built and used both with sequential and multithreaded
 Geant4 builds. In case of multithreaded Geant4 toolkit, the application will
 run in proper multithreaded mode.

 Run the executable with the --help option to see the available options:
 
``` bash
 -m   <Geant4-Macro-File> [MANDATORY; a standard Geant4 macro file name]
 -f   <Physics-List-Name> [OPTIONAL;  physics list name (default: FTFP_BERT)]
 -p   <NO-ARGUMENT>       [OPTIONAL;  run in performance mode (default: no)]
 -g : flag  ==> build the detector from a GDML file (default: false -> build from SQLite file)
 -d : the GDML/SQLite file name (default: ATLAS-R2-2016-01-00-01)
``` 
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



# Examples

 To execute the application using the <macro.g4> macro file, with the FTFP_BERT
 Physics List, in performance mode and building the detector from the default SQLite file:
``` bash
 ./fullSimLight -m macro.g4 -f FTFP_BERT -p 
 ``` 
 To execute the application using the <macro.g4> macro file, with the FTFP_BERT
 Physics List, not in performance mode and building the detector from the default SQLite file:
 ``` bash
 ./fullSimLight -m macro.g4 -f FTFP_BERT -g  
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
