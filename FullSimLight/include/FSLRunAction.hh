
#ifndef FSLRunAction_h
#define FSLRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4String.hh"

//G4AnalysisMananager
#include "FSLAnalysis.hh"

#include "GeantinoMapsConfigurator.hh"

class FSLRun;
class G4Timer;
class FSLSteppingAction;
class FSLTrackingAction;


class FSLRunAction: public G4UserRunAction {

public:

  FSLRunAction();
 ~FSLRunAction();

  G4Run* GenerateRun() override;
  void   BeginOfRunAction(const G4Run*) override;
  void   EndOfRunAction(const G4Run*) override;

  void   SetPerformanceFlag(G4bool val)    { fIsPerformance = val; }
  void   SetPythiaConfig(G4String config)  { fPythiaConfig  = config; }

  void  SetSteppingAction(FSLSteppingAction* stpact) { fSteppingAction = stpact; }
  void  SetTrackingAction(FSLTrackingAction* tract)  { fTrackingAction = tract;  }

  void  SetSpecialScoringRegionName(const G4String& rname) { fSpecialScoringRegionName = rname; }

private:
  GeantinoMapsConfigurator* fGeantinoMapsConf;
  G4bool            fIsPerformance;
  //G4bool            fIsGeantino;
  FSLRun*            fRun;
  G4Timer*          fTimer;
  FSLSteppingAction* fSteppingAction;
  FSLTrackingAction* fTrackingAction;
  //G4String          fGeantinoMapsFilename;
  G4String          fPythiaConfig;
  G4String          fSpecialScoringRegionName;

    //TO DO: make private and add Get methods
public:

    static G4AnalysisManager* fMasterAnalysisManager;
    // ID of the RadLen 2D Profile
    int fRadName_id;
    // ID of the IntLen 2D Profile
    int fIntName_id;
    // ID of the Eta RadLen 1D Profile
    int fEtaRad_id;
    // ID of the Eta IntLen 1D Profile
    int fEtaInt_id;
};

#endif
