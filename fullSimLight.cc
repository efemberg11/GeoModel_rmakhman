
//--------------------------------------------------------
// fullSimLight application: 17 September 2019 (README)
//--------------------------------------------------------


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
#include "MyGVPhysicsList.hh"

#include "MyActionInitialization.hh"

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

static bool         parIsPerformance = false;
static bool         parBuildGDML = false;
static std::string  parGDMLFileName = "ATLAS-R2-2016-01-00-01.gdml";
static std::string  parSQLiteFileName = "ATLAS-R2-2016-01-00-01.db";
static std::string  parMacroFileName = "";
static std::string  parPhysListName  = "FTFP_BERT";

void GetInputArguments(int argc, char** argv);
void Help();


int main(int argc, char** argv) {
  // get input arguments
  GetInputArguments(argc, argv);
  G4cout<< " ========== Running FullSimLight ========================= " << G4endl
        << "   Physics list name   =  " << parPhysListName           << G4endl
        << "   Geant4 macro        =  " << parMacroFileName          << G4endl
        << "   Build from GDML     =  " << parBuildGDML              << G4endl;
  if(parBuildGDML)
      G4cout<< "   Geometry File       =  " << parGDMLFileName           << G4endl;
  else
      G4cout<< "   Geometry File       =  " << parSQLiteFileName         << G4endl;
  G4cout<< "   Performance mode    =  " << parIsPerformance  << G4endl
        << " ===================================================== " << G4endl;
  //
  //choose the Random engine: set to MixMax explicitely (default form 10.4)
  G4Random::setTheEngine(new CLHEP::MixMaxRng);
  // set seed and print info
  G4Random::setTheSeed(12345678);
  G4cout << G4endl
  << " ===================================================== " << G4endl
  << " Random engine = " << G4Random::getTheEngine()->name()   << G4endl
  << " Initial seed  = " << G4Random::getTheSeed()             << G4endl
  << " ===================================================== " << G4endl
  << G4endl;
    //
    // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  // Number of threads can be defined via macro command
  G4int nThreads = 4;
  runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager* runManager = new G4RunManager;
#endif
  //
  // set mandatory initialization classes
  //
  // 1. Detector construction
  MyDetectorConstruction* detector = new MyDetectorConstruction;
  if (parBuildGDML){
     detector->SetGDMLFileName(parGDMLFileName);
     detector->SetBuildFromGDML(true);
  }
  else
     detector->SetSQLiteFileName(parSQLiteFileName);
  runManager->SetUserInitialization(detector);
  //
  // 2. Physics list
  G4PhysListFactory factory;
  if (factory.IsReferencePhysList(parPhysListName)) {
    G4VModularPhysicsList* physList = factory.GetReferencePhysList(parPhysListName);
    runManager->SetUserInitialization(physList);
  } else if (parPhysListName==G4String("GV")) {
    G4VUserPhysicsList* physList = new MyGVPhysicsList();
    runManager->SetUserInitialization(physList);
  } else {
    G4cerr << "ERROR: Physics List " << parPhysListName << " UNKNOWN!" << G4endl;
    return -1;
  }
  //
  // 3. User action
  runManager->SetUserInitialization(new MyActionInitialization(parIsPerformance));
  //
  // 4. Run the simulation in batch mode
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4String command = "/control/execute ";
  UI->ApplyCommand(command+parMacroFileName);
  //
  // Print out the final random number
  G4cout << G4endl
      	 << " ================================================================= " << G4endl
         << " Final random number = " << CLHEP::HepRandom::getTheEngine()->flat() << G4endl
      	 << " ================================================================= " << G4endl
         << G4endl;
  //
  // Delete the RunManager
  delete runManager;
  return 0;
}




static struct option options[] = {
  {"standard Geant4 macro file (REQUIRED)"                           , required_argument, 0, 'm'},
  {"physics list name (default FTFP_BERT)"                                  , required_argument, 0, 'f'},
  {"flag to run the application in performance mode (default FALSE)" , no_argument      , 0, 'p'},
  {"flag to build the detector from GDML file(default FALSE)"        , no_argument      , 0, 'g'},
  {"GDML or SQLite file name" , required_argument, 0, 'd'},
  {0, 0, 0, 0}
};


void Help() {
  std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
  std::cout <<"  FullSimLight Geant4 application.    \n"
            << std::endl
            <<"  **** Parameters: \n"
            <<"      -m :   REQUIRED : the standard Geant4 macro file name \n"
            <<"      -f :   physics list name (default: FTFP_BERT) \n"
            <<"      -p :   flag  ==> run the application in performance mode i.e. no user actions \n"
            <<"         :   -     ==> run the application in NON performance mode i.e. with user actions (default) \n"
            <<"      -g :   flag  ==> build the detector from a GDML file (default: false -> build from SQLite file)\n"
            <<"      -d :   the GDML/SQLite file name (default: ATLAS-R2-2016-01-00-01)\n"
            << std::endl;
  std::cout <<"\nUsage: fullSimLight [OPTIONS] INPUT_FILE\n\n" <<std::endl;
  for (int i=0; options[i].name!=NULL; i++) {
    printf("\t-%c  --%s\t%s\n", options[i].val, options[i].name, options[i].has_arg ? options[i].name : "");
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
   c = getopt_long(argc, argv, "pm:f:gd:", options, &optidx);
   if (c == -1)
     break;
   //
   switch (c) {
   case 0:
     c = options[optidx].val;
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
     parBuildGDML     = true;
     break;
   case 'd':
     if(parBuildGDML)
         parGDMLFileName  = optarg;
     else
         parSQLiteFileName  = optarg;
     break;
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
  // check if build from GDML file flag is set that a GDML file was provided
  if (parBuildGDML && parGDMLFileName=="") {
        G4cout << "  *** ERROR : GDML file is required. " << G4endl;
        Help();
        exit(-1);
    }
}
