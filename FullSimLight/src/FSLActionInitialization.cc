
#include "FSLActionInitialization.hh"
#include "GeoModelKernel/GeoPluginLoader.h"
#include "FSLPrimaryGeneratorAction.hh"
#include "FSLRunAction.hh"
#include "FSLEventAction.hh"
#include "FSLSteppingAction.hh"
#include "FSLTrackingAction.hh"
#include "PythiaPrimaryGeneratorAction.hh"

#include "FSLUserRunActionPlugin.h"
#include "FSLUserEventActionPlugin.h"
#include "FSLUserStackingActionPlugin.h"
#include "FSLUserTrackingActionPlugin.h"
#include "FSLUserSteppingActionPlugin.h"


#include "G4Version.hh"
#if G4VERSION_NUMBER>=1040
#include "G4MultiRunAction.hh"
#include "G4MultiEventAction.hh"
#include "G4MultiTrackingAction.hh"
#include "G4MultiSteppingAction.hh"
#include "FSLLengthIntegratorEventAction.hh"
#include "FSLLengthIntegratorSteppingAction.hh"
#endif



//const G4AnalysisManager* FSLActionInitialization::fMasterAnalysisManager = nullptr;

FSLActionInitialization::FSLActionInitialization(bool isperformance, bool customuseractions)
: G4VUserActionInitialization(), fIsPerformance(isperformance),fCustomUserActions(customuseractions),
  fSpecialScoringRegionName("") {
      
      fGeantinoMapsConfig = GeantinoMapsConfigurator::getGeantinoMapsConf();
  }


FSLActionInitialization::~FSLActionInitialization() {}

// called in case of MT
void FSLActionInitialization::BuildForMaster() const {
    
    if(fCustomUserActions){}/*set run action from config file*/
    else
    {FSLRunAction* masterRunAct = new FSLRunAction();
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
}


void FSLActionInitialization::Build() const {

#if !USE_PYTHIA
  SetUserAction(new FSLPrimaryGeneratorAction());
#else
  if (use_pythia()) {
    // seed each generator/thread by 1234 if perfomance mode run and use the event
    // ID+1 as seed otherwise (guaranted reproducibility while having different events)
    G4int pythiaSeed = fIsPerformance ? -1 : 0;
    SetUserAction(new PythiaPrimaryGeneratorAction(pythiaSeed));
  } else {
    SetUserAction(new FSLPrimaryGeneratorAction());
  }
#endif

#ifndef G4MULTITHREADED
// in sequential mode the BuildForMaster method is not called:
// - create the only one run action with perfomance flag true i.e. only time is measured
  if (fIsPerformance) {
    FSLRunAction* masterRunAct = new FSLRunAction();
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
  if (!fIsPerformance && !fCustomUserActions) {
      FSLRunAction* runact = new FSLRunAction();
      SetUserAction(runact);
      runact->SetSpecialScoringRegionName(fSpecialScoringRegionName);


      if(!fGeantinoMapsConfig->GetCreateGeantinoMaps()){
          FSLEventAction*    evtAct = new FSLEventAction();
          FSLTrackingAction*  trAct = new FSLTrackingAction(evtAct);
          FSLSteppingAction* stpAct = new FSLSteppingAction(evtAct);
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
          G4UA::FSLLengthIntegratorSteppingAction* FSLLenghtIntSteppingAct = new G4UA::FSLLengthIntegratorSteppingAction(runact);
          //Event action
          G4UA::FSLLengthIntegratorEventAction* FSLLenghtIntEventAct = new G4UA::FSLLengthIntegratorEventAction(FSLLenghtIntSteppingAct, runact);
          SetUserAction(FSLLenghtIntEventAct);
          SetUserAction(FSLLenghtIntSteppingAct);

      }
#endif
      //MultiEventActions?? TO DO?
  }
    
  else if(fCustomUserActions)
  {
      for (const auto& element : runActions)
      {
          GeoPluginLoader<FSLUserRunActionPlugin> loader;
          const FSLUserRunActionPlugin * plugin = loader.load(element);
          G4UserRunAction*  runAct = plugin->getUserRunAction();
          SetUserAction(runAct);
          
      }
      
      for (const auto& element : eventActions)
      {
          GeoPluginLoader<FSLUserEventActionPlugin> loader;
          const FSLUserEventActionPlugin * plugin = loader.load(element);
          G4UserEventAction*  evtAct = plugin->getUserEventAction();
          SetUserAction(evtAct);
          
      }
      
      for (const auto& element : steppingActions)
      {
          GeoPluginLoader<FSLUserSteppingActionPlugin> loader;
          const FSLUserSteppingActionPlugin * plugin = loader.load(element);
          G4UserSteppingAction*  steAct = plugin->getUserSteppingAction();
          SetUserAction(steAct);
          
      }
      
      
      for (const auto& element : trackingActions)
      {
          GeoPluginLoader<FSLUserTrackingActionPlugin> loader;
          const FSLUserTrackingActionPlugin * plugin = loader.load(element);
          G4UserTrackingAction*  traAct = plugin->getUserTrackingAction();
          SetUserAction(traAct);
          
      }
      
      for (const auto& element : stackingActions)
      {
          GeoPluginLoader<FSLUserStackingActionPlugin> loader;
          const FSLUserStackingActionPlugin * plugin = loader.load(element);
          G4UserStackingAction*  staAct = plugin->getUserStackingAction();
          SetUserAction(staAct);
          
      }
      
      
      
      
      
      
      
  }
  
}
