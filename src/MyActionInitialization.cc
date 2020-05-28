
#include "MyActionInitialization.hh"

#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"
#include "MyTrackingAction.hh"
#include "LengthIntegrator.hh"
#include "MyLengthIntegratorEventAction.hh"
#include "MyLengthIntegratorSteppingAction.hh"

#include "G4MultiRunAction.hh"
#include "G4MultiEventAction.hh"
#include "G4MultiTrackingAction.hh"
#include "G4MultiSteppingAction.hh"


MyActionInitialization::MyActionInitialization(bool isperformance, bool createGeantinoMaps)
: G4VUserActionInitialization(), fIsPerformance(isperformance), fCreateGeantinoMaps(createGeantinoMaps) {}


MyActionInitialization::~MyActionInitialization() {}

// called in case of MT
void MyActionInitialization::BuildForMaster() const {
    MyRunAction* masterRunAct = new MyRunAction();
    masterRunAct->SetPerformanceFlag(fIsPerformance);
    SetUserAction(masterRunAct);
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
      SetUserAction(new MyRunAction());
      
      if(!fCreateGeantinoMaps){
          std::cout<<"fCreateGeantinoMaps is FALSE"<<std::endl;
          MyEventAction* evtact = new MyEventAction();
          SetUserAction(evtact);
          SetUserAction(new MyTrackingAction(evtact));
          SetUserAction(new MySteppingAction(evtact));
          
      }
      else
      {
          
          std::cout<<"fCreateGeantinoMaps is TRUE, will create geantino Maps"<<std::endl;
          G4UA::MyLengthIntegratorSteppingAction* myLenghtIntSteppingAct = new G4UA::MyLengthIntegratorSteppingAction();
          G4UA::MyLengthIntegratorEventAction* myLenghtIntEventAct = new G4UA::MyLengthIntegratorEventAction(myLenghtIntSteppingAct);
          SetUserAction(myLenghtIntEventAct);
          SetUserAction(myLenghtIntSteppingAct);
      }
    
      //MultiEventActions?? TO DO?
  }
}
