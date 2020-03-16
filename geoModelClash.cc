
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
#include "MyGVPhysicsList.hh"

#include "MyActionInitialization.hh"

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

static G4String parMacroFileName = "macro.g4";
static G4String geometryFileName   = "geometry-ATLAS-R2-2016-01-00-01.db";
static G4String outputFileName     = "gmclash_report";

void GetInputArguments(int argc, char** argv);
void Help();


int main(int argc, char** argv) {
    
    // Get input arguments
    GetInputArguments(argc, argv);
    
    G4cout
    << " ===================  Running GeoModelClash =================== "      << G4endl
    << "   Geometry file name               =  " << geometryFileName         << G4endl
    << "   Output clashes report file name  =  " << outputFileName           << G4endl
    << " ============================================================== "      << G4endl;
    
    //choose the Random engine: set to MixMax explicitely (default form 10.4)
    G4Random::setTheEngine(new CLHEP::MixMaxRng);
    // set seed and print info
    G4Random::setTheSeed(12345678);
//    G4cout << G4endl
//    << " ===================================================== "      << G4endl
//    << "   Random engine      = " << G4Random::getTheEngine()->name() << G4endl
//    << "   Initial seed       = " << G4Random::getTheSeed()           << G4endl
//    << " ===================================================== "      << G4endl
//    << G4endl;
    
    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    // Number of threads can be defined via macro command
    G4int nThreads = 4;
    runManager->SetNumberOfThreads(nThreads);
#else
    G4RunManager* runManager = new G4RunManager;
#endif
    
    // set mandatory initialization classes
    // 1. Detector construction
    MyDetectorConstruction* detector = new MyDetectorConstruction;
    detector->SetRunOverlapCheck(true);
        
    detector->SetGeometryFileName (geometryFileName);
    runManager->SetUserInitialization(detector);
  
//    // 2. Physics list
//    G4PhysListFactory factory;
//    if (factory.IsReferencePhysList(parPhysListName)) {
//        G4VModularPhysicsList* physList = factory.GetReferencePhysList(parPhysListName);
//        runManager->SetUserInitialization(physList);
//    } else if (parPhysListName==G4String("GV")) {
//        G4VUserPhysicsList* physList = new MyGVPhysicsList();
//        runManager->SetUserInitialization(physList);
//    } else {
//        G4cerr << "ERROR: Physics List " << parPhysListName << " UNKNOWN!" << G4endl;
//        return -1;
//    }
  
    // 3. User action
    //runManager->SetUserInitialization(new MyActionInitialization(parIsPerformance));
  
    // 4. Run the simulation in batch mode
    G4UImanager* UI = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    UI->ApplyCommand(command+parMacroFileName);
  
    // Print out the final random number
//    G4cout << G4endl
//           << " ================================================================= " << G4endl
//           << " Final random number = " << CLHEP::HepRandom::getTheEngine()->flat() << G4endl
//             << " ================================================================= " << G4endl
//           << G4endl;
//    //
    // Delete the RunManager
    delete runManager;
    return 0;
}

static struct option options[] = {
    {"geometry file name       "  , required_argument, 0, 'g'},
    {"output clashes report file name "  , required_argument, 0, 'o'},
    {0, 0, 0, 0}
};


void Help() {
  std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
  G4cout <<"  GeoModelClash Geant4 application.    \n"
            << std::endl
            <<"  **** Parameters: \n\n"
            <<"      -g :   the Geometry file name (default: geometry-ATLAS-R2-2016-01-00-01.db)\n"
            <<"      -o :   clashes report file name (default: gmclash_report)\n"
            << std::endl;
  std::cout <<"\nUsage: ./gmclash [OPTIONS] -m <MACRO_FILE>\n" <<std::endl;
  for (int i=0; options[i].name!=NULL; i++) {
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
   c = getopt_long(argc, argv, "pm:f:g:o", options, &optidx);
   if (c == -1)
     break;
   //
   switch (c) {
   case 0:
     c = options[optidx].val;
     break;
   case 'g':
     geometryFileName = optarg;
     break;
   case 'o':
     outputFileName = optarg;
     break;
   default:
     Help();
     errx(1, "unknown option %c", c);
   }
  }
}
