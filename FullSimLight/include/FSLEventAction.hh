
#ifndef FSLEventAction_h
#define FSLEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "FSLEventData.hh"

class G4Event;
class G4Track;

class FSLEventAction: public G4UserEventAction {

public:

   FSLEventAction();
  ~FSLEventAction() override;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

  void AddData(G4double edep, G4double length, G4bool ischarged, G4bool isspecial);
  void AddSecondaryTrack(const G4Track* track, G4bool isspecial);

  void SetIsSpecialScoringRegion(G4bool val) { fIsSpecialScoring = val; }

private:

  G4bool      fIsSpecialScoring;  

  FSLEventData fEventData;
  FSLEventData fEventDataSpecialRegion;

};

#endif
