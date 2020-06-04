
#include "MyActionInitialization.hh"

#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"
#include "MyTrackingAction.hh"
#include "MyLengthIntegratorEventAction.hh"
#include "MyLengthIntegratorSteppingAction.hh"

#include "G4MultiRunAction.hh"
#include "G4MultiEventAction.hh"
#include "G4MultiTrackingAction.hh"
#include "G4MultiSteppingAction.hh"


//const G4AnalysisManager* MyActionInitialization::fMasterAnalysisManager = nullptr;

MyActionInitialization::MyActionInitialization(bool isperformance, bool createGeantinoMaps)
: G4VUserActionInitialization(), fIsPerformance(isperformance), fCreateGeantinoMaps(createGeantinoMaps) {}


MyActionInitialization::~MyActionInitialization() {}

// called in case of MT
void MyActionInitialization::BuildForMaster() const {
    MyRunAction* masterRunAct = new MyRunAction();
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    SetUserAction(masterRunAct);
    
//    std::cout<<"MyActionInitialization::BuildForMaster(): taking the Master G4AnalysisManager::Instance"<<std::endl;
//    fMasterAnalysisManager = G4AnalysisManager::Instance();
//    std::cout<<"fMasterAnalysisManager: "<<fMasterAnalysisManager<<std::endl;
    //exit(-1);
}


void MyActionInitialization::Build() const {
  SetUserAction(new MyPrimaryGeneratorAction());
#ifndef G4MULTITHREADED
// in sequential mode the BuildForMaster method is not called:
// - create the only one run action with perfomance flag true i.e. only time is measued
  if (fIsPerformance) {
    MyRunAction* masterRunAct = new MyRunAction();
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    SetUserAction(masterRunAct);
  }
#endif
  // do not create Run,Event,Stepping and Tracking actions in case of perfomance mode
  if (!fIsPerformance) {
      MyRunAction* runact = new MyRunAction();
      SetUserAction(runact);
      
      if(!fCreateGeantinoMaps){
          MyEventAction* evtact = new MyEventAction();
          SetUserAction(evtact);
          SetUserAction(new MyTrackingAction(evtact));
          SetUserAction(new MySteppingAction(evtact));
          
      }
      else
      {
          G4UA::MyLengthIntegratorSteppingAction* myLenghtIntSteppingAct = new G4UA::MyLengthIntegratorSteppingAction(runact);
          G4UA::MyLengthIntegratorEventAction* myLenghtIntEventAct = new G4UA::MyLengthIntegratorEventAction(myLenghtIntSteppingAct, runact);
          SetUserAction(myLenghtIntEventAct);
          SetUserAction(myLenghtIntSteppingAct);
          
      }
      //MultiEventActions?? TO DO?
  }
}
