
//--------------------------------------------------------
// fullSimLight application: 17 September 2019 (README)
//--------------------------------------------------------

#include "G4Types.hh"
#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"

#include "G4PhysListFactory.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"

#include "Randomize.hh"
#include "MyDetectorConstruction.hh"
#include "StandardEmWithWoodcock.hh"
#include "EmExtraPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "MyActionInitialization.hh"
#include "PythiaPrimaryGeneratorAction.hh"

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

static bool         parIsPerformance   = false;
static G4String     geometryFileName   ;
static std::string  parMacroFileName   = "";
static std::string  parPhysListName    = "FTFP_BERT";
static bool         parRunOverlapCheck = false;

void GetInputArguments(int argc, char** argv);
void Help();


int main(int argc, char** argv) {

  // JFB if the G4 environment does not already set path to these variables, look for
  // them in standard places. 
  const std::string g4ShareDir=G4SHAREDIR;
  setenv("G4NEUTRONHPDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4NDL4.6").c_str(),                0);
  setenv("G4LEDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4EMLOW7.9.1").c_str(),                   0);
  setenv("G4LEVELGAMMADATA",(g4ShareDir+"/Geant4-10.6.1/data/PhotonEvaporation5.5").c_str(),   0);
  setenv("G4RADIOACTIVEDATA",(g4ShareDir+"/Geant4-10.6.1/data/RadioactiveDecay5.4").c_str(),   0);
  setenv("G4PARTICLEXSDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4PARTICLEXS2.1").c_str(),        0);
  setenv("G4PIIDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4PII1.3").c_str(),                      0);
  setenv("G4REALSURFACEDATA",(g4ShareDir+"/Geant4-10.6.1/data/RealSurface2.1.1").c_str() ,     0);
  setenv("G4SAIDXSDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4SAIDDATA2.0").c_str(),              0);
  setenv("G4ABLADATA",(g4ShareDir+"/Geant4-10.6.1/data/G4ABLA3.1").c_str(),                    0);
  setenv("G4INCLDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4INCL1.0").c_str(),                    0);
  setenv("G4ENSDFSTATEDATA",(g4ShareDir+"/Geant4-10.6.1/data/G4ENSDFSTATE2.2").c_str(),        0);
  
    // Get input arguments
    GetInputArguments(argc, argv);

    G4cout
    << " =============== Running FullSimLight ================ "      << G4endl
    << "   Physics list name  =  " << parPhysListName                 << G4endl
    << "   Geant4 macro       =  " << parMacroFileName                << G4endl
    << "   Performance mode   =  " << parIsPerformance                << G4endl
    << "   Geometry file      =  " << geometryFileName                << G4endl
    << "   Run Overlap Check  =  " << parRunOverlapCheck              << G4endl
    << " ===================================================== "      << G4endl;

    G4Timer myTotalCPUTimer;
    myTotalCPUTimer.Start();

    //choose the Random engine: set to MixMax explicitely (default form 10.4)
    G4Random::setTheEngine(new CLHEP::MixMaxRng);
    // set seed and print info
    G4Random::setTheSeed(12345678);
    G4cout << G4endl
    << " ===================================================== "      << G4endl
    << "   Random engine      = " << G4Random::getTheEngine()->name() << G4endl
    << "   Initial seed       = " << G4Random::getTheSeed()           << G4endl
    << " ===================================================== "      << G4endl
    << G4endl;

    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    // Number of threads can be defined via macro command
    G4int nThreads = 4;
    runManager->SetNumberOfThreads(nThreads);
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    // 1. Physics list
    G4bool activateRegions = false;
    G4VModularPhysicsList* physList = nullptr;
    G4PhysListFactory factory;
    if (factory.IsReferencePhysList(parPhysListName)) {
        physList = factory.GetReferencePhysList(parPhysListName);
    } else if (parPhysListName==G4String("FTFP_BERT_ATL_WDCK")) {
        G4cout << "<<< Geant4 FTFP_BERT_ATL physics list with the local Woodcock settings " << G4endl;
        physList = factory.GetReferencePhysList("FTFP_BERT_ATL");
        // the local em-standard physics with Woodcock tracking for gamma
        StandardEmWithWoodcock* em0AndWDCK = new StandardEmWithWoodcock;
        // set the region name and low energy limit for Woodcock tracking
        em0AndWDCK->SetRegionNameForWoodcockTracking("EMEC");
        em0AndWDCK->SetLowEnergyLimitForWoodcockTracking(200.0*CLHEP::keV);
        physList->ReplacePhysics(em0AndWDCK);
        // the local version of the `G4EmExtraPhysics` that will use the local `GammaGeneralProcess`
        G4VPhysicsConstructor* emExtra = new EmExtraPhysics;
        physList->ReplacePhysics(emExtra);
        //physList->RemovePhysics("G4GammaLeptoNuclearPhys");
        // make sure that regions will also be added to the detector
        activateRegions = true;
    } else {
        G4cerr << "ERROR: Physics List " << parPhysListName << " UNKNOWN!" << G4endl;
        return -1;
    }
    // In cases of ATLAS physics lists, set the neutron tracking cut to be 150 [ns] as in Athena
    if (parPhysListName.find("ATL") != std::string::npos) {
      G4NeutronTrackingCut* neutronCut = new G4NeutronTrackingCut("neutronCutphysics", 1);
      neutronCut->SetTimeLimit(150.0*CLHEP::ns);
      physList->ReplacePhysics(neutronCut);
    }
    
    // register the final version of the physics list in the run manager
    runManager->SetUserInitialization(physList);

    // 2. Detector construction
    MyDetectorConstruction* detector = new MyDetectorConstruction;

    if (parRunOverlapCheck) detector->SetRunOverlapCheck(true);
    if (activateRegions)    detector->SetAddRegions(true);

    detector->SetGeometryFileName (geometryFileName);
    runManager->SetUserInitialization(detector);

    // 3. User action
    MyActionInitialization* actInit = new MyActionInitialization(parIsPerformance);
    // set the name of a region in which we are interested to see a very basic simulation
    // stat e.g. "EMEC" (NOTE: only if the given region can be found and executed in
    // non-perfomance mode)
    const G4String nameSpecialScoringRegion = ""; //"EMEC"
    actInit->SetSpecialScoringRegionName(nameSpecialScoringRegion);
    runManager->SetUserInitialization(actInit);

    // 4. Run the simulation in batch mode
    G4UImanager* UI = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    UI->ApplyCommand(command+parMacroFileName);

    // Print out the final random number
    G4cout << G4endl
           << " ================================================================= " << G4endl
           << " Final random number = " << CLHEP::HepRandom::getTheEngine()->flat() << G4endl
      	   << " ================================================================= " << G4endl
           << G4endl;
    //
    // Delete the RunManager
    delete runManager;
    myTotalCPUTimer.Stop();
    G4cout << "FullSimLight done! Total execution time info: " << G4endl;
    G4cout << "*** User time elapsed   : " <<myTotalCPUTimer.GetUserElapsed()   << G4endl;
    G4cout << "*** Real time elapsed   : " <<myTotalCPUTimer.GetRealElapsed()   << G4endl;
    G4cout << "*** System time elapsed : " <<myTotalCPUTimer.GetSystemElapsed() << G4endl
    << " ================================================================= "    << G4endl;
    return 0;
}

static struct option options[] = {
    {"macro file               "  , required_argument, 0, 'm'},
    {"physics list name        "  , required_argument, 0, 'f'},
    {"performance flag         "  , no_argument      , 0, 'p'},
    {"geometry file name       "  , required_argument, 0, 'g'},
    {"pythia primary generator "  , required_argument, 0, 'P'},
    {"overlap geometry check   "  , no_argument      , 0, 'o'},
    {"help"                       , no_argument      , 0, 'h'},
    {0, 0, 0, 0}
};


void Help() {
  std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
  G4cout <<"  FullSimLight Geant4 application.    \n"
            << std::endl
            <<"  **** Parameters: \n\n"
            <<"      -m :   REQUIRED : the standard Geant4 macro file name \n"
            <<"      -g :   REQUIRED : the Geometry file name \n"
            <<"      -o :   flag  ==> run the geometry overlap check (default: FALSE)\n"
            <<"      -f :   physics list name (default: FTFP_BERT) \n"
            <<"      -P :   use Pythia primary generator [config. available: ttbar/higgs/minbias or use a Pythia command input file]\n"
            <<"      -p :   flag  ==> run the application in performance mode i.e. no user actions \n"
            <<"         :   -     ==> run the application in NON performance mode i.e. with user actions (default) \n"<< std::endl;

  std::cout <<"\nUsage: ./fullSimLight [OPTIONS] -m <MACRO_FILE>\n" <<std::endl;
  for (int i=0; options[i].name!=NULL; i++) {
    //printf("\t-%c  --%s\t%s\n", options[i].val, options[i].name, options[i].has_arg ? options[i].name : "");
    printf("\t-%c  --%s\t\n", options[i].val, options[i].name);
  }
  std::cout<<"\n "<<std::setw(100)<<std::setfill('=')<<""<<std::setfill(' ')<<std::endl;
}

void GetInputArguments(int argc, char** argv) {
  // process arguments
  if (argc == 1) {
   Help();
   exit(0);
  }
  while (true) {
   int c, optidx = 0;
   c = getopt_long(argc, argv, "P:pm:f:g:oh", options, &optidx);
   if (c == -1)
     break;
   //
   switch (c) {
   case 0:
     c = options[optidx].val;
     break;
   case 'P':
#if USE_PYTHIA
     set_pythia_config(optarg);
#else
     std::cerr << "Support for Pythia is not available. \nPlease visit the website http://home.thep.lu.se/Pythia/ to install it in your system." << std::endl;
     exit(1);
#endif
     break;
   case 'p':
     parIsPerformance = true;
     break;
   case 'm':
     parMacroFileName = optarg;
     break;
   case 'f':
     parPhysListName  = optarg;
     break;
   case 'g':
     geometryFileName = optarg;
     break;
   case 'o':
     parRunOverlapCheck = true;
     break;
   case 'h':
     Help();
     exit(0);
   default:
     Help();
     errx(1, "unknown option %c", c);
   }
  }
  // check if mandatory Geant4 macro file was provided
  if (parMacroFileName=="") {
      G4cout << "  *** ERROR : Geant4 macro file is required. " << G4endl;
      Help();
      exit(-1);
  }
  // check if mandatory Geometry file was provided
  if (geometryFileName=="") {
      G4cout << "  *** ERROR : Geometry file is required. " << G4endl;
      Help();
      exit(-1);
    }
}
