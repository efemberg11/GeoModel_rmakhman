
#ifndef FSLTrackingAction_h
#define FSLTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;
class FSLEventAction;
class G4Region;

class FSLTrackingAction : public G4UserTrackingAction {

public:

   FSLTrackingAction(FSLEventAction*);
  ~FSLTrackingAction() override;

   void SetScoringRegion(G4Region* reg);

   void PreUserTrackingAction(const G4Track*) override;

private:

  FSLEventAction* fEventAction;
  G4Region*      fScoringRegion;

};

#endif
