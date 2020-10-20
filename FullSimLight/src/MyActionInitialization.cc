
#include "MyActionInitialization.hh"

#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"
#include "MyTrackingAction.hh"
#include "MyLengthIntegratorEventAction.hh"
#include "MyLengthIntegratorSteppingAction.hh"
#include "PythiaPrimaryGeneratorAction.hh"

#include "G4MultiRunAction.hh"
#include "G4MultiEventAction.hh"
#include "G4MultiTrackingAction.hh"
#include "G4MultiSteppingAction.hh"


//const G4AnalysisManager* MyActionInitialization::fMasterAnalysisManager = nullptr;

MyActionInitialization::MyActionInitialization(bool isperformance, bool createGeantinoMaps, G4String geantinoMapsFilename)
: G4VUserActionInitialization(), fIsPerformance(isperformance), fCreateGeantinoMaps(createGeantinoMaps),fGeantinoMapsFilename(geantinoMapsFilename){}


MyActionInitialization::~MyActionInitialization() {}

// called in case of MT
void MyActionInitialization::BuildForMaster() const {
    MyRunAction* masterRunAct = new MyRunAction(fCreateGeantinoMaps,fGeantinoMapsFilename);
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    SetUserAction(masterRunAct);
}


void MyActionInitialization::Build() const {

#if !USE_PYTHIA
  SetUserAction(new MyPrimaryGeneratorAction());
#else
  if (use_pythia())
    SetUserAction(new PythiaPrimaryGeneratorAction());
  else
    SetUserAction(new MyPrimaryGeneratorAction());
#endif

#ifndef G4MULTITHREADED
// in sequential mode the BuildForMaster method is not called:
// - create the only one run action with perfomance flag true i.e. only time is measured
  if (fIsPerformance) {
    MyRunAction* masterRunAct = new MyRunAction(fCreateGeantinoMaps, fGeantinoMapsFilename);
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    SetUserAction(masterRunAct);
  }
#endif
  // do not create Run,Event,Stepping and Tracking actions in case of perfomance mode
  if (!fIsPerformance) {
      MyRunAction* runact = new MyRunAction(fCreateGeantinoMaps, fGeantinoMapsFilename);
      SetUserAction(runact);
      
      if(!fCreateGeantinoMaps){
          MyEventAction* evtact = new MyEventAction();
          SetUserAction(evtact);
          SetUserAction(new MyTrackingAction(evtact));
          SetUserAction(new MySteppingAction(evtact));
          
      }
      else
      {
          //Stepping action
          G4UA::MyLengthIntegratorSteppingAction* myLenghtIntSteppingAct = new G4UA::MyLengthIntegratorSteppingAction(runact);
          myLenghtIntSteppingAct->SetRlimit(fRlimit);
          myLenghtIntSteppingAct->SetZlimit(fZlimit);
          myLenghtIntSteppingAct->SetXlimit(fXlimit);
          myLenghtIntSteppingAct->SetYlimit(fYlimit);
          myLenghtIntSteppingAct->SetCreateDetectorsMaps(fCreateDetectorsMaps);
          myLenghtIntSteppingAct->SetCreateMaterialsMaps(fCreateMaterialsMaps);
          myLenghtIntSteppingAct->SetCreateElementsMaps(fCreateElementsMaps);
          //Event action
          G4UA::MyLengthIntegratorEventAction* myLenghtIntEventAct = new G4UA::MyLengthIntegratorEventAction(myLenghtIntSteppingAct, runact);
          myLenghtIntEventAct->SetCreateEtaPhiMaps(fCreateEtaPhiMaps);
          SetUserAction(myLenghtIntEventAct);
          SetUserAction(myLenghtIntSteppingAct);
          
      }
      //MultiEventActions?? TO DO?
  }
}
