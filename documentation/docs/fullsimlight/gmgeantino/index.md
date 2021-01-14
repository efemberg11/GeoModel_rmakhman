# GeoModelGeantino

GeoModelGeantino (gmgeantino) is a Geant4 based application that allows you to produce geantino maps for the geometry specified as input. It supports .db/.gdml/.dylib/.so geometry formats and it writes out the geantino maps in a ROOT file. However, it does not depend on ROOT, cause it uses the G4AnalysisManager to create/fill/write 1D and 2D Profiles. Examples of 1D and 2D profiles can be seen in Figure 1 and Figure 2, respectively.

{{ imgutils_image_caption('1D_profile.png', 
   alt='1Dprofile', 
   cap='Figure 1 : 1D Profiles filled per Event.',
   urlFix=False) 
}}

{{ imgutils_image_caption('2D_profile.png', 
   alt='2Dprofile', 
   cap='Figure 2 : 2D Profiles filled per Step.',
   urlFix=False) 
}}


The 1D and 2D profiles are filled during the simulation, per Event or per Step. The creation of different profiles can be tuned with command line flags. In general X-Y, Z-R, eta, phi RadiationLength/InteractionLength profiles can be created per DetectorVolume/Material/Element.


*gmgeantino* can be run on different subdetectors and the results be combined to obtain plots like the one in Figure 3, that represents the material distribution expressed in units of *X0* as a function of *η* for the ATLAS Inner Detector.  The breakdown indicates the contributions of external services and of individual sub-detectors, including services in their active volume.

{{ imgutils_image_caption('MaterialDistribution.png', 
   alt='materialDistr', 
   cap='Figure 3 : Material distribution expressed in units of *X0* as a function of *η* for the ATLAS Inner Detector.',
   urlFix=False) 
}}

gmgeantino uses a default geant4 macro, called *geantino.g4*, to take some input parameters. You should find this macro under your *&lt install-path &gt/share/FullSimLight* directory. You can edit this macro to change some parameters, like the verbosity, the n. of threads, the n. of primaries per event, the primary particle energy.. etc. 

By default the primary particles shot by 'gmgeantino' are geantinos (this parameter should not be changed). The default number of simulated geantinos is set to 1000. To increase the resolution of the maps, the n. of simulated events can be increased, editing the macro and changing the value at the following line:

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

To produce geantino maps of a geometry described by one of the [GeoModelPlugins repo](https://gitlab.cern.ch/atlas/GeoModelPlugins), i.e.  *AGDDPlugin*, using a custom macro file *mymacro.g4*, activate detectors/materials and elements maps, and write out the geantino maps in the *geantinoMaps_AGDD.root* file :
``` bash
./gmgeantino -m mymacro.g4 -g libAGDDPlugin.1.0.0.dylib -d -a -l -o geantinoMaps_AGDD.root 
```
Examples of geantino maps generated running on the AGDD plugin can be seen in Figure 4 and Figure 5. They shows resplectively the total R-Z radiation length profile and the X-Y radiation length profile for the element Aluminium. 

{{ imgutils_image_caption('AGDD_RZRadLen.png', 
   alt='agdd_1', 
   cap='Figure 4 : Total R-Z radiation length profile of the AGDD geometry',
   urlFix=False) 
}}

{{ imgutils_image_caption('AGDD_XYRadLen_Aluminium.png', 
   alt='agdd_2', 
   cap='Figure 5 : X-Y radiation length profile for the element Aluminium in the AGDD geometry',
   urlFix=False) 
}}




