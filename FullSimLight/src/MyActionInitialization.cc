
#include "MyActionInitialization.hh"

#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"
#include "MyTrackingAction.hh"
#include "PythiaPrimaryGeneratorAction.hh"

#include "G4Version.hh"
#if G4VERSION_NUMBER>=1040
#include "G4MultiRunAction.hh"
#include "G4MultiEventAction.hh"
#include "G4MultiTrackingAction.hh"
#include "G4MultiSteppingAction.hh"
#include "MyLengthIntegratorEventAction.hh"
#include "MyLengthIntegratorSteppingAction.hh"
#endif



//const G4AnalysisManager* MyActionInitialization::fMasterAnalysisManager = nullptr;

MyActionInitialization::MyActionInitialization(bool isperformance)
: G4VUserActionInitialization(), fIsPerformance(isperformance),
  fSpecialScoringRegionName("") {
      
      fGeantinoMapsConfig = GeantinoMapsConfigurator::getGeantinoMapsConf();
  }


MyActionInitialization::~MyActionInitialization() {}

// called in case of MT
void MyActionInitialization::BuildForMaster() const {
    
    MyRunAction* masterRunAct = new MyRunAction();
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    masterRunAct->SetSpecialScoringRegionName(fSpecialScoringRegionName);

#if USE_PYTHIA
    if (use_pythia()) {
      G4String str(get_pythia_config());
      masterRunAct->SetPythiaConfig(str);
    }
#endif
    SetUserAction(masterRunAct);
}


void MyActionInitialization::Build() const {

#if !USE_PYTHIA
  SetUserAction(new MyPrimaryGeneratorAction());
#else
  if (use_pythia()) {
    // seed each generator/thread by 1234 if perfomance mode run and use the event
    // ID+1 as seed otherwise (guaranted reproducibility while having different events)
    G4int pythiaSeed = fIsPerformance ? -1 : 0;
    SetUserAction(new PythiaPrimaryGeneratorAction(pythiaSeed));
  } else {
    SetUserAction(new MyPrimaryGeneratorAction());
  }
#endif

#ifndef G4MULTITHREADED
// in sequential mode the BuildForMaster method is not called:
// - create the only one run action with perfomance flag true i.e. only time is measured
  if (fIsPerformance) {
    MyRunAction* masterRunAct = new MyRunAction();
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    masterRunAct->SetSpecialScoringRegionName(fSpecialScoringRegionName);
#if USE_PYTHIA
    if (use_pythia()) {
      G4String str(get_pythia_config());
      masterRunAct->SetPythiaConfig(str);
    }
#endif
    SetUserAction(masterRunAct);
  }
#endif
  // do not create Run,Event,Stepping and Tracking actions in case of perfomance mode
  if (!fIsPerformance) {
      MyRunAction* runact = new MyRunAction();
      SetUserAction(runact);
      runact->SetSpecialScoringRegionName(fSpecialScoringRegionName);


      if(!fGeantinoMapsConfig->GetCreateGeantinoMaps()){
          MyEventAction*    evtAct = new MyEventAction();
          MyTrackingAction*  trAct = new MyTrackingAction(evtAct);
          MySteppingAction* stpAct = new MySteppingAction(evtAct);
          SetUserAction(evtAct);
          SetUserAction(trAct);
          SetUserAction(stpAct);
          runact->SetTrackingAction(trAct);
          runact->SetSteppingAction(stpAct);
      }

#if G4VERSION_NUMBER>=1040
      else
      {

          //Stepping action
          G4UA::MyLengthIntegratorSteppingAction* myLenghtIntSteppingAct = new G4UA::MyLengthIntegratorSteppingAction(runact);
          //Event action
          G4UA::MyLengthIntegratorEventAction* myLenghtIntEventAct = new G4UA::MyLengthIntegratorEventAction(myLenghtIntSteppingAct, runact);
          //myLenghtIntEventAct->SetCreateEtaPhiMaps(fCreateEtaPhiMaps);
          SetUserAction(myLenghtIntEventAct);
          SetUserAction(myLenghtIntSteppingAct);

      }
#endif
      //MultiEventActions?? TO DO?
  }
}
