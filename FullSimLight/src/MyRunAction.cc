#include "MyRunAction.hh"

#include "G4Version.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4Timer.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "MyRun.hh"


#include "G4ProductionCutsTable.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

G4AnalysisManager* MyRunAction::fMasterAnalysisManager = nullptr;

MyRunAction::MyRunAction(bool isGeantino, G4String geantinoMapFilename)
: G4UserRunAction(), fIsPerformance(false), fIsGeantino(isGeantino),
  fRun(nullptr), fTimer(nullptr), fGeantinoMapsFilename(geantinoMapFilename),
  fPythiaConfig("") { }

MyRunAction::~MyRunAction() {
    if(fIsGeantino)
        delete G4AnalysisManager::Instance();
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


void MyRunAction::BeginOfRunAction(const G4Run* /*aRun*/){

#if G4VERSION_NUMBER>=1040

    if(fIsGeantino)
    {
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
        G4cout<<"\n\nBeginOfRunAction: \n...create a root file using the G4AnalysisManager" << G4endl;
        if (!analysisManager->OpenFile(fGeantinoMapsFilename)){
            G4cout<<"\nBeginOfRunAction ERROR: File cannot be opened!"<<G4endl;
            exit(-1);
        } else
            G4cout<<"\n...output File "<<fGeantinoMapsFilename<<" opened!"<<G4endl;

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
        const char* etaRadName = "etaRadLen";
        if(analysisManager->GetP1Id(etaRadName, false)< 0)
        {
            fEtaRad_id = analysisManager->CreateP1(etaRadName,etaRadName,500,-6,6);
            analysisManager->SetP1XAxisTitle(fEtaRad_id,"#eta");
            analysisManager->SetP1YAxisTitle(fEtaRad_id,"Radiation Length %X0");

        }
        const char* etaIntName = "etaIntLen";
        if(analysisManager->GetP1Id(etaIntName, false)< 0)
        {
            fEtaInt_id = analysisManager->CreateP1(etaIntName,etaIntName,500,-6,6);
            analysisManager->SetP1XAxisTitle(fEtaInt_id,"#eta");
            analysisManager->SetP1YAxisTitle(fEtaInt_id,"Interaction Length #lambda");
        }

    }
#endif
    if (isMaster) {

        //G4cout<<"\nBeginOfRunAction isMaster, and fMasterAnalysisManager: "<<fMasterAnalysisManager<<G4endl;

        G4cout << "\n ========================  Material-cuts couples ========================== \n" << G4endl;
        std::vector<G4Region*>* regionVector = G4RegionStore::GetInstance();
        std::size_t numTotalUsedCouples = 0;
        for (std::size_t ir=0, nr=regionVector->size(); ir<nr; ++ir) {
          G4Region* reg = (*regionVector)[ir];
          G4cout << " ---> Region name: = " << reg->GetName() << G4endl;
          const std::size_t numMats = reg->GetNumberOfMaterials();
          std::vector<G4Material*>::const_iterator mItr = reg->GetMaterialIterator();
          std::size_t numUsedCouples = 0;
          for (std::size_t im=0; im<numMats; ++im) {
            const G4Material*              mat = (*mItr);
            const G4MaterialCutsCouple* couple = reg->FindCouple(const_cast<G4Material*>(mat));
            if (couple && couple->IsUsed()) {
              ++numUsedCouples;
            }
            ++mItr;
          }
          numTotalUsedCouples += numUsedCouples;
          G4cout << "     -- " << numUsedCouples << " mat-cuts couples used inside " << G4endl;
        }
        G4cout << "     = Total number of mat-cuts couple used: " << numTotalUsedCouples << G4endl;
        G4cout << "\n ==================================  DONE!  =============================== \n" << G4endl;


        G4String  msg0 = (fPythiaConfig == "") ? "a Particle-Gun" : "PYTHIA (" + fPythiaConfig +")";
#ifdef G4MULTITHREADED
        G4int nThreads = G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads();
        G4int nEvent   = G4MTRunManager::GetMasterRunManager()->GetNumberOfEventsToBeProcessed();
        G4cout << "\n  =======================================================================================\n"
        << "   Run started in MT mode with " << nThreads << "  threads, simulating " << nEvent << " events using " << msg0 << " \n"
        << "  =======================================================================================  \n"
        << G4endl;
#else
        G4int nEvent   = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
        G4cout << "\n  =======================================================================================\n"
        << "   Run started in sequential mode (i.e. with 1 thread), simulating " << nEvent << " events using " << msg0 << " \n"
        << "  =======================================================================================  \n"
        << G4endl;
#endif
        fTimer = new G4Timer();
        fTimer->Start();
    }
    //else G4cout<<"BeginOfRunAction isWorker, and fMasterAnalysisManager: "<<fMasterAnalysisManager<<G4endl;
}


void MyRunAction::EndOfRunAction(const G4Run*) {

#if G4VERSION_NUMBER>=1040

    if(fIsGeantino){

        auto analysisManager= G4AnalysisManager::Instance();
        //Finalize analysisManager and Write out file
        if (analysisManager->IsOpenFile()){
            G4cout<<"\n\n EndOfRunAction, writing output file: "<<analysisManager->GetFileName() << G4endl;
            analysisManager->Write();
            G4cout<<"... closing output file: "<<analysisManager->GetFileName() << G4endl;
            analysisManager->CloseFile();
            G4cout<<"Output File successfully saved and closed! " << G4endl;
        }

    }
#endif

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

        if (!fIsPerformance) { // otherwise we do not even create any MyRun objects so fRun is nullptr
            fRun->EndOfRun();
        }
    }
}
