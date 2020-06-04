#include "MyRunAction.hh"

#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif
#include "G4Timer.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "MyRun.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4ProductionCutsTable.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

G4AnalysisManager* MyRunAction::fMasterAnalysisManager = nullptr;

MyRunAction::MyRunAction() : G4UserRunAction(), fIsPerformance(false), fRun(nullptr), fTimer(nullptr) {
    
}

MyRunAction::~MyRunAction() {
    //if (isMaster) {
        delete G4AnalysisManager::Instance();
    //}
}

G4Run* MyRunAction::GenerateRun() {
  // don't Generate our Run in perfomance mode but return with nullptr:
  //   the RunManager::RunInitialization will create a base G4Run object if this method gives back nullptr
  if (!fIsPerformance) {
    fRun = new MyRun();
    return fRun;
  }
  return nullptr;
}


void MyRunAction::BeginOfRunAction(const G4Run* /*aRun*/) {
    
    // Create analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (isMaster) {
        fMasterAnalysisManager = analysisManager;
        //G4cout<<"MyRunAction::BeginOfRunAction, created MASTER istance of the G4AnalysisManager: "<<fMasterAnalysisManager<<G4endl;
        
    }
    //else
    //    G4cout<<"MyRunAction::BeginOfRunAction, created WORKER istance of the G4AnalysisManager: "<<analysisManager<<G4endl;
    
    G4cout << "Using G4AnalysisManager type: " << analysisManager->GetType() << G4endl;
    analysisManager->SetVerboseLevel(1);
    
    // Open output root file
    std::string fileName_g4 = "geantinoMaps.root";
    G4cout<<"\n\nBeginOfRunAction, create root file with the G4AnalysisManager " << G4endl;
    if (!analysisManager->OpenFile(fileName_g4)){
        G4cout<<"\nBeginOfRunAction ERROR: File cannot be opened!"<<G4endl;
        exit(-1);
    } else
        G4cout<<"\nBeginOfRunAction FILE "<<fileName_g4<<" opened!"<<G4endl;
    
    const char* radName = "RZRadLen";
    
    if(analysisManager->GetP2Id(radName, false) < 0){
        fRadName_id = analysisManager->CreateP2(radName,radName,1000,-25000.,25000.,2000,0.,15000.);
        //G4cout<<"MyRunAction::BeginOfRunAction: G4AnalysisManager Created RZRadLen 2DProfile with name: "<<radName<< " and  with id: "<<fRadName_id<<G4endl;
        analysisManager->SetP2XAxisTitle(fRadName_id,"Z[mm]");
        analysisManager->SetP2YAxisTitle(fRadName_id,"R[mm]");
        analysisManager->SetP2ZAxisTitle(fRadName_id,"thickstepRL");
        
    }
    const char* intName = "RZIntLen";
    if(analysisManager->GetP2Id(intName, false)< 0)
    {
        fIntName_id = analysisManager->CreateP2(intName,intName,1000,-25000.,25000.,2000,0.,15000.);
        //G4cout<<"MyRunAction::BeginOfRunAction: G4AnalysisManager Created RZIntLen 2DProfile with name: "<<intName<< " and with id: "<<fIntName_id<<G4endl;
        analysisManager->SetP2XAxisTitle(fIntName_id,"Z[mm]");
        analysisManager->SetP2YAxisTitle(fIntName_id,"R[mm]");
        analysisManager->SetP2ZAxisTitle(fIntName_id,"thickstepIL");
    }
    if (isMaster) {
      
      //G4cout<<"\nBeginOfRunAction isMaster, and fMasterAnalysisManager: "<<fMasterAnalysisManager<<G4endl;
      
      std::vector<G4Region*>* regionVect =  G4RegionStore::GetInstance();
      int numRegions = regionVect->size();
      int sumNumMC = 0;
      G4ProductionCutsTable* pTable = G4ProductionCutsTable::GetProductionCutsTable();
      size_t numMC = pTable->GetTableSize();
      std::vector<int> mcRVect(23,0);
      for (size_t imc=0; imc<numMC; ++imc) {
          const G4MaterialCutsCouple* mc =pTable->GetMaterialCutsCouple(imc);
          if (!mc->IsUsed()) continue;
          G4Material* mat = const_cast<G4Material*>(mc->GetMaterial());
          int k=0;
          for (; k<numRegions && (!(*regionVect)[k]->FindCouple(mat)); ++k){}
              if (k<numRegions) {
                ++mcRVect[k];
              }
      }
      for (int ir=0; ir<numRegions; ++ir) {
          G4cout<< " ir = " << ir << "  region Name = " << (*regionVect)[ir]->GetName() <<" #mc = " << mcRVect[ir] << G4endl;
          sumNumMC += mcRVect[ir];
      }
      G4cout<< " === Total number of MC = " << sumNumMC << " vs " << numMC << " #regions = " << numRegions << G4endl;
      
      
#ifdef G4MULTITHREADED
    G4int nThreads = G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads();
    G4cout << "\n  =======================================================================================\n"
           << "   Run started in MT mode with " << nThreads << "  threads    \n"
           << "  =======================================================================================  \n"
           << G4endl;
#else
    G4cout << "\n  =======================================================================================\n"
           << "   Run started in sequential mode (i.e. with 1 thread)        \n"
           << "  =======================================================================================  \n"
           << G4endl;
#endif
    fTimer = new G4Timer();
    fTimer->Start();
  }
    //else G4cout<<"BeginOfRunAction isWorker, and fMasterAnalysisManager: "<<fMasterAnalysisManager<<G4endl;
}


void MyRunAction::EndOfRunAction(const G4Run*) {
    
    auto analysisManager= G4AnalysisManager::Instance();
    //Finalize analysisManager and Write out file
    if (analysisManager->IsOpenFile()){
        G4cout<<"\n\n EndOfRunAction, write output file: "<<analysisManager->GetFileName() << G4endl;
        analysisManager->Write();
        analysisManager->CloseFile();
        G4cout<<"Output File successfully saved and closed! " << G4endl;
    }

  if (isMaster) {
    fTimer->Stop();
    // get number of events: even in case of perfomance mode when MyRun-s are not generated in GenerateRun()
#ifdef G4MULTITHREADED
    const G4Run* run = G4MTRunManager::GetMasterRunManager()->GetCurrentRun();
#else
    const G4Run* run = G4RunManager::GetRunManager()->GetCurrentRun();
#endif
    G4int nEvt   = run->GetNumberOfEvent();
    G4cout << "\n\n" << G4endl;
    G4cout << "  =======================================================================================  " << G4endl;
    G4cout << "   Run terminated:                                                                         " << G4endl;
    G4cout << "     Number of events transported    = " << nEvt                                             << G4endl;
    G4cout << "     Time:  "  << *fTimer                                                                   << G4endl;
    G4cout << "  =======================================================================================  "<< G4endl;
    delete fTimer;
    // print primary gun properties (not available at the begining of the run)
    MyPrimaryGeneratorAction::Print();
    if (!fIsPerformance) { // otherwise we do not even create any MyRun objects so fRun is nullptr
        fRun->EndOfRun();
    }
  }
}
