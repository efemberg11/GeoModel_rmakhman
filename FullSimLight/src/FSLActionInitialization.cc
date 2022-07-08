
#include "FSLActionInitialization.hh"
#include "GeoModelKernel/GeoPluginLoader.h"
#include "FSLPrimaryGeneratorAction.hh"
#include "FSLRunAction.hh"
#include "FSLEventAction.hh"
#include "FSLSteppingAction.hh"
#include "FSLTrackingAction.hh"
#include "PythiaPrimaryGeneratorAction.hh"
#include "FSLUserActionPlugin.h"
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
    
    //set run action from config file
    if(fCustomUserActions){
        for (const std::string & element: userActions) {
            GeoPluginLoader<FSLUserActionPlugin> loader;
            const FSLUserActionPlugin * plugin = loader.load(element);
            if (plugin->getRunAction()) SetUserAction(plugin->getRunAction());
        }
        
    }
    else
    {
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
    for (const std::string & element: userActions) {
      GeoPluginLoader<FSLUserActionPlugin> loader;
      const FSLUserActionPlugin * plugin = loader.load(element);
      
      if (plugin->getEventAction()) SetUserAction(plugin->getEventAction()); 
      if (plugin->getRunAction()) SetUserAction(plugin->getRunAction()); 
      if (plugin->getTrackingAction()) SetUserAction(plugin->getTrackingAction()); 
      if (plugin->getStackingAction()) SetUserAction(plugin->getStackingAction()); 
      if (plugin->getSteppingAction()) SetUserAction(plugin->getSteppingAction()); 

    }
  }
}
