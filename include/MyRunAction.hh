
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

  MyRunAction();
  virtual ~MyRunAction();

  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

  void    SetPerformanceFlag(G4bool val) { fIsPerformance=val;   }

private:
  G4bool       fIsPerformance;
  MyRun*       fRun;
  G4Timer*     fTimer;

    //TO DO: make private and add Get methods
public:
    
    static G4AnalysisManager* fMasterAnalysisManager;
    int fRadName_id;
    int fIntName_id;
};

#endif
