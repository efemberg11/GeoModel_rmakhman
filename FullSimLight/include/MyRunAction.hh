
#ifndef MyRunAction_h
#define MyRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4String.hh"

//G4AnalysisMananager
#include "MyAnalysis.hh"

class MyRun;
class G4Timer;


class MyRunAction: public G4UserRunAction {

public:

  MyRunAction(bool isGeantino, G4String geantinoMapsFileName="geantinoMaps.root");
 ~MyRunAction();

  G4Run* GenerateRun() override;
  void   BeginOfRunAction(const G4Run*) override;
  void   EndOfRunAction(const G4Run*) override;

  void   SetPerformanceFlag(G4bool val)    { fIsPerformance = val; }
  void   SetPythiaConfig(G4String config)  { fPythiaConfig  = config; }
private:

  G4bool       fIsPerformance;
  G4bool       fIsGeantino;
  MyRun*       fRun;
  G4Timer*     fTimer;
  G4String     fGeantinoMapsFilename;
  G4String     fPythiaConfig;


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
