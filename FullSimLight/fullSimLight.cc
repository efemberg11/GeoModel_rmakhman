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


#include "Randomize.hh"
#include "FSLDetectorConstruction.hh"
#include "FSLPhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

#include "GeoModelKernel/GeoPluginLoader.h"

#include "FSLActionInitialization.hh"
#include "FSLConfigurator.hh"
#include "PythiaPrimaryGeneratorAction.hh"
#include "FullSimLight/FSLUserActionPlugin.h"
#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

#include <sys/types.h>
#include <dirent.h>

#include <nlohmann/json.hpp>
#include <fstream>


static const std::string fullSimLightShareDir=FULLSIMLIGHTSHAREDIR;
static std::string  parMacroFileName   = fullSimLightShareDir+"/macro.g4";
static bool         parIsPerformance   = false;
static bool         parIsCustomUserActions = false;
static G4String     geometryFileName   ;
static std::string  parPhysListName    = "FTFP_BERT";
static bool         parRunOverlapCheck = false;
bool isBatch = true;
static std::string  parConfigFileName  = "config.json";



void GetInputArguments(int argc, char** argv);
void Help();


int main(int argc, char** argv) {
    
    // JFB if the G4 environment does not already set path to these variables, look for
    // them in standard places.
    
    auto dataSetEnv=[] (const std::string &dir, const std::string & dataSetEnvName, const std::string & dataset) {
        if (getenv(dataSetEnvName.c_str())) {
            std::cout << dataSetEnvName << "=" << getenv(dataSetEnvName.c_str()) << std::endl;
        }
        else {
            DIR *directory = opendir(dir.c_str());
            if (directory) {
                dirent * entry = readdir(directory);
                while (entry) {
                    std::string entryName=entry->d_name;
                    if (entryName.find(dataset)!=std::string::npos) {
                        std::cout << dataSetEnvName << "=" << (dir+"/"+entryName) << std::endl;
                        setenv(dataSetEnvName.c_str(),(dir+"/"+entryName).c_str(),0);
                    }
                    entry=readdir(directory);
                }
                closedir(directory);
            }
        }
    };
    
    
    const std::string g4ShareDir=G4SHAREDIR;
    const std::string g4Version=G4VERSION;
    const std::string searchDir=g4ShareDir+"/Geant4-"+g4Version+"/data";
    
    
    G4cout
    << " =============== Running FullSimLight ================ "      << G4endl;
    
    // Get input arguments
    GetInputArguments(argc, argv);
    
    if(isBatch){
        G4cout
        << "   Command line parameters"                                    << G4endl
        << "   Physics list name  =   " << parPhysListName                 << G4endl
        << "   Geant4 macro       =   " << parMacroFileName                << G4endl
        << "   Performance mode   =   " << parIsPerformance                << G4endl
        << "   Geometry file      =   " << geometryFileName                << G4endl
        << "   Run Overlap Check  =   " << parRunOverlapCheck              << G4endl
        << " ===================================================== \n"     << G4endl;
        
    }
    
    G4cout
    << " == Geant4 Environment Variables == "      << G4endl;
    dataSetEnv(searchDir,"G4NEUTRONHPDATA", "G4NDL");
    dataSetEnv(searchDir,"G4LEDATA","G4EMLOW");
    dataSetEnv(searchDir,"G4LEVELGAMMADATA","PhotonEvaporation");
    dataSetEnv(searchDir,"G4RADIOACTIVEDATA","RadioactiveDecay");
    dataSetEnv(searchDir,"G4PARTICLEXSDATA","G4PARTICLEXS");
    dataSetEnv(searchDir,"G4PIIDATA","G4PII");
    dataSetEnv(searchDir,"G4REALSURFACEDATA","RealSurface");
    dataSetEnv(searchDir,"G4SAIDXSDATA","G4SAIDDATA");
    dataSetEnv(searchDir,"G4ABLADATA","G4ABLA");
    dataSetEnv(searchDir,"G4INCLDATA","G4INCL");
    dataSetEnv(searchDir,"G4ENSDFSTATEDATA","G4ENSDFSTATE");
    G4cout<< " ===================================================== " << G4endl;
    
    
    G4Timer FSLTotalCPUTimer;
    FSLTotalCPUTimer.Start();
    
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
    if(!isBatch){
        
        simConfig::parse_json_file(parConfigFileName);
        parPhysListName = simConfig::fsl.physicsList;
        
        
        if(simConfig::fsl.eventGeneratorName=="Pythia")
        {
#if USE_PYTHIA
        if(simConfig::fsl.typeOfEvent != "") set_pythia_config((simConfig::fsl.typeOfEvent).c_str());
        else set_pythia_config((simConfig::fsl.eventInputFile).c_str());

#else
        std::cerr << "Support for Pythia is not available. \nPlease visit the website http://home.thep.lu.se/Pythia/ to install it in your system." << std::endl;
        exit(1);
            
#endif
        }
    }

    const FSLPhysListFactory *phyListFactory = FSLPhysListFactory::GetInstance();
    G4VModularPhysicsList *physList = phyListFactory->GetPhysList(parPhysListName);
    G4bool activateRegions = phyListFactory->GetActivateRegionsFlag();
    
    // register the final version of the physics list in the run manager
    runManager->SetUserInitialization(physList);
    
    // 2. Detector construction
    FSLDetectorConstruction* detector = new FSLDetectorConstruction;
    
    if (parRunOverlapCheck) detector->SetRunOverlapCheck(true);
    if (activateRegions)    detector->SetAddRegions(true);
    
    // 3. User action
    if(!isBatch && simConfig::fsl.userActions.size()>0) parIsCustomUserActions = true;
    
    
    FSLActionInitialization* actInit = new FSLActionInitialization(parIsPerformance,parIsCustomUserActions);
    
    if(parIsCustomUserActions){
    actInit->SetActions(
                        simConfig::fsl.userActions
                       );

    }
    
    if(!isBatch)
    {
        actInit->SetGenerator(simConfig::fsl.eventGeneratorName);
        actInit->SetHepMC3FilePath(simConfig::fsl.hepmc3InputFile);
        actInit->SetHepMC3FileType(simConfig::fsl.hepmc3TypeOfFile);
        actInit->SetGeneratorPlugin(simConfig::fsl.generatorPlugin);
    }

    // set the name of a region in which we are interested to see a very basic simulation
    // stat e.g. "EMEC" (NOTE: only if the given region can be found and executed in
    // non-perfomance mode)
    const G4String nameSpecialScoringRegion = ""; //"EMEC"
    actInit->SetSpecialScoringRegionName(nameSpecialScoringRegion);
    runManager->SetUserInitialization(actInit);
    
    // 4. Run the simulation in batch mode
    G4UImanager* UI = G4UImanager::GetUIpointer();
    
    if(isBatch){
        
        detector->SetGeometryFileName (geometryFileName);
        runManager->SetUserInitialization(detector);
        
        G4String command = "/control/execute ";
        UI->ApplyCommand(command+parMacroFileName);
        
    } else
    {
        
      //  std::ifstream ifs(parConfigFileName);
       // auto jf=json::parse(ifs);
        
      //  simConfig::fslConfig fsl;
      //  simConfig::regionConfig rc;
        
        //read and store the configuration into the fslConfig struct
      //  simConfig::from_json(jf, fsl);
        
        
        
        detector->SetGeometryFileName (simConfig::fsl.geometry);
        runManager->SetUserInitialization(detector);
        
        if(simConfig::fsl.magFieldType=="Map")
        {
        detector->SetMagFieldPluginPath(simConfig::fsl.magFieldPlugin);
        detector->SetMagFieldMapPath(simConfig::fsl.magFieldMap);
        }
                
        
        //parse RegionsData
        std::vector<std::string> Regions;
        std::vector<std::vector<G4String>> RootLVNames;
        std::vector<double> electron_cut;
        std::vector<double> proton_cut;
        std::vector<double> positron_cut;
        std::vector<double> gamma_cut;
        
        for (const auto& element : simConfig::jf["Regions data"]){
            
//            std::cout<<"RegionName: "<<element["RegionName"]<<std::endl;
//            std::cout<<"RootLVNames: "<<element["RootLVNames"]<<std::endl;
//            std::cout<<"GammaCut: "<<element["GammaCut"]<<std::endl;
//            std::cout<<"ElectronCut: "<<element["ElectronCut"]<<std::endl;
//            std::cout<<"PositronCut: "<<element["PositronCut"]<<std::endl;
//            std::cout<<"ProtonCut: "<<element["ProtonCut"]<<std::endl;
            
            //actually read the data and save them in a simConfig::regionConfig object (might be useful for the configuration later on)
            
            simConfig::from_json(element, simConfig::rc);
            Regions.push_back(simConfig::rc.regionName);
            RootLVNames.push_back(simConfig::rc.rootLVNames);
            electron_cut.push_back(simConfig::rc.electronCut);
            proton_cut.push_back(simConfig::rc.protonCut);
            positron_cut.push_back(simConfig::rc.positronCut);
            gamma_cut.push_back(simConfig::rc.gammaCut);
            
//            std::cout<<"RegionName: "<<rc.regionName<<std::endl;
//            std::cout<<"RootLVNames size: "<<rc.rootLVNames.size()<<std::endl;
//            std::cout<<"GammaCut: "<<rc.gammaCut<<std::endl;
//            std::cout<<"ElectronCut: "<<rc.electronCut<<std::endl;
//            std::cout<<"PositronCut: "<<rc.positronCut<<std::endl;
//            std::cout<<"ProtonCut: "<<rc.protonCut<<std::endl;
//            std::cout<<"------------------------------------------------"<<std::endl;
        }
        
        detector->ConfigureRegionsFSL(Regions, RootLVNames, electron_cut, proton_cut, positron_cut, gamma_cut);
        
        for (const auto& element : simConfig::jf["Sensitive Detector Extensions"]){
            
            detector->AddSensitiveDetectorPlugin(element);
        }


        G4bool initialized=false;
        //parse and apply G4Commands
        for (G4String element : simConfig::jf["g4ui_commands"]){
            //std::cout<<"Applying G4Commands: "<<element<<std::endl;
            //Initialize the Geant4 kernel before applying the first FSL user interface command
            if(element.contains("FSLgun") && !initialized) {
                runManager->Initialize();
                initialized=true;
            }
            UI->ApplyCommand(element);
        }
        
        G4cout
        << " ===================================================== "      << G4endl
        << "   Configuration parameters"                                  << G4endl
        << "   Geometry file      =  " << simConfig::fsl.geometry                    << G4endl
        << "   Physics list name  =  " << simConfig::fsl.physicsList                 << G4endl
        << "   Generator          =  " << simConfig::fsl.eventGeneratorName          << G4endl
        << "   Magnetic Field     =  " << simConfig::fsl.magFieldType           << G4endl
        << "   Run Overlap Check  =  " << parRunOverlapCheck              << G4endl
        << " ===================================================== "      << G4endl;
        
        
        // Initialize the G4 kernel if it hasn't been initialized
        if (!initialized) runManager->Initialize();
        runManager->BeamOn(simConfig::fsl.nEvents);
    }
    
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
    FSLTotalCPUTimer.Stop();
    G4cout << "FullSimLight done! Total execution time info: " << G4endl;
    G4cout << "*** User time elapsed   : " <<FSLTotalCPUTimer.GetUserElapsed()   << G4endl;
    G4cout << "*** Real time elapsed   : " <<FSLTotalCPUTimer.GetRealElapsed()   << G4endl;
    G4cout << "*** System time elapsed : " <<FSLTotalCPUTimer.GetSystemElapsed() << G4endl
    << " ================================================================= "    << G4endl;
    return 0;
}

static struct option options[] = {
    {"physics list name        "  , required_argument, 0, 'f'},
    {"configuration file       "  , required_argument, 0, 'c'},
    {"macro file               "  , required_argument, 0, 'm'},
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
    <<"  ** Please specify a json configuration file with the -c flag or use the following command line parameters : \n\n"
    <<"  **** Command line parameters: \n\n"
    <<"      -g :   REQUIRED : the Geometry file name \n"
    <<"      -m :   the Geant4 macro file name \n"
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
        c = getopt_long(argc, argv, "P:pm:f:g:c:oh", options, &optidx);
        if (c == -1)
            break;
        //
        switch (c) {
            case 0:
                c = options[optidx].val;
                break;
            case 'P':
#if USE_PYTHIA
                parMacroFileName   = fullSimLightShareDir+"/pythia.g4";
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
            case 'c':
                isBatch = false;
                parConfigFileName = optarg;
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
    // check if mandatory Geometry file was provided
    if (geometryFileName=="" && isBatch) {
        G4cout << "  *** ERROR : Geometry file is required. Please provide it in a json configuration file or via command line using the -g flag " << G4endl;
        Help();
        exit(-1);
    }
}
